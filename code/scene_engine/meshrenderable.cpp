#include "meshrenderable.h"
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/shaderprogram.h>
#include <renderer_engine/rendercontext.h>
#include <scene_models/visitorapplytransformations.h>
#include <scene_models/visitorgetmeshnodes.h>
#include <scene_models/visitorgetmeshnodesunderspecifictransformation.h>
#include <scene_models/visitorapplyanimation.h>
#include <scene_models/visitorgettransformationnode.h>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/export.hpp>

#include <fstream>
#include <functional>

#include <scene_models/meshnodemodel.h>
#include <scene_models/transformationnodemodel.h>
#include <config/config.h>
#include "converters.h"

BOOST_CLASS_EXPORT(sa::MeshNodeModel)
BOOST_CLASS_EXPORT(sa::TransformationNodeModel);

namespace sa {
MeshRenderable::MeshRenderable(const std::string& resourcePath, const std::string& resourceName)
  : m_resourcePath(resourcePath)
  , m_resourceName(resourceName) {
  std::ifstream ifs((m_resourcePath + m_resourceName + ".header").c_str(), std::ios_base::binary);
  if(ifs.good())
  {
    boost::archive::binary_iarchive ia(ifs);

    //We only load the header here.
    ia >> BOOST_SERIALIZATION_NVP(m_meshModel.m_header);
  }
}

const AABBModel& MeshRenderable::getBoundingBox() const {
  return m_meshModel.m_header.boundingBox;
}

void MeshRenderable::toCPU(ImageCache& imageCache) {
  std::ifstream ifs((m_resourcePath + m_resourceName + ".data").c_str(), std::ios_base::binary);
  if(ifs.good()) {
    boost::archive::binary_iarchive ia(ifs);
    ia >> BOOST_SERIALIZATION_NVP(m_meshModel.m_data);
  }

  unsigned int materialKey = 0;
  for(MeshModel::Data::Materials::value_type material : m_meshModel.m_data.m_materials) {
    if(!material->texDirAmbient().empty()) {
      Image img;
      if(!imageCache.try_get(m_resourcePath + material->texDirAmbient(), img)) {
        img = Image(m_resourcePath + material->texDirAmbient());
        imageCache.insert(m_resourcePath + material->texDirAmbient(), img);
      }
      m_ambientImage[materialKey] = img;
    }
    if(!material->texDirDiffuse().empty()) {
      Image img;
      if(!imageCache.try_get(m_resourcePath + material->texDirDiffuse(), img)) {
        img = Image(m_resourcePath + material->texDirDiffuse());
        imageCache.insert(m_resourcePath + material->texDirDiffuse(), img);
      }
      m_diffuseImage[materialKey] = img;
    }
    if(!material->texDirSpecular().empty()) {
      Image img;
      if(!imageCache.try_get(m_resourcePath + material->texDirSpecular(), img)) {
        img = Image(m_resourcePath + material->texDirSpecular());
        imageCache.insert(m_resourcePath + material->texDirSpecular(), img);
      }
      m_specularImage[materialKey] = img;
    }
    ++materialKey;
    //m_ambientImage[materialE.first] = Image(m_resourcePath + material->texDirAmbient());
    //m_diffuseImage[materialE.first] = Image(m_resourcePath + material->texDirDiffuse());
    //m_specularImage[materialE.first] = Image(m_resourcePath + material->texDirSpecular());
  }
  //m_currentDataStorage = DataStorage::CPU;
}

void MeshRenderable::toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context) {
  //Setup the shader paths.
  std::set<std::string> defines;



  defines.insert("NUMBER_OF_CASCADES " + std::to_string(numberOfShadowCascades));
  for(MeshModel::Data::Materials::value_type material : m_meshModel.m_data.m_materials) {
    if(!material->texDirAmbient().empty()) {
      defines.insert("AMBIENT_TEXTURE");
    }
    if(!material->texDirDiffuse().empty()) {
      defines.insert("DIFFUSE_TEXTURE");
    }
    if(!material->texDirSpecular().empty()) {
      defines.insert("SPECULAR_TEXTURE");
    }
  }

  //Tell the shader that the model is bone animated if thats the case.
  if(m_meshModel.m_data.m_haveBones) {
    defines.insert("BONE_ANIMATION");
  }

  ShaderProgramPtr sp;
  std::string vshDir = (config.getParam("DATA_DIR") + "/shaders/" + "ubershader.vsh");
  std::string fshDir = (config.getParam("DATA_DIR") + "/shaders/" + "ubershader.fsh");

  auto shaderkey = std::make_tuple(defines, vshDir, fshDir);
  if(!shaderCache.try_get(shaderkey, sp)) {
    sp = device->createShaderProgramFromFile(
          vshDir.c_str(),
          fshDir.c_str(),
          defines);
    shaderCache.insert(shaderkey, sp);
  }

  int posAttr = sp->attributeLocation("posAttr");
  int texAttr = sp->attributeLocation("texAttr");
  int norAttr = sp->attributeLocation("norAttr");
  int bAttr = sp->attributeLocation("bAttr");
  int wAttr = sp->attributeLocation("wAttr");

  m_modelMatrixUniform = sp->uniformLocation("u_modelMatrix");

  ShaderProgramPtr ssp = device->createShaderProgramFromFile(
        (config.getParam("DATA_DIR") + "/shaders/" + "ubershadowshader.vsh").c_str(),
        (config.getParam("DATA_DIR") + "/shaders/" + "ubershadowshader.fsh").c_str(),
        defines);


  sa::VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
    {norAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {bAttr, sa::VertexDescriptionElement::Type::FLOAT, 4},
    {wAttr, sa::VertexDescriptionElement::Type::FLOAT, 4}
  };

  unsigned int meshIndex = 0;
  for(sa::MeshModel::Data::SubMeshes::value_type sm : m_meshModel.m_data.m_subMeshes) {
    DrawData subMeshDrawData;
    const MaterialModel* material = m_meshModel.m_data.m_materials[sm->getMaterialKey()];

    sa::TexturePtr ambientTex;
    sa::TexturePtr diffuseTex;
    sa::TexturePtr speculaTex;
    textureCache.try_get(material->texDirAmbient(), ambientTex);
    textureCache.try_get(material->texDirDiffuse(), diffuseTex);
    textureCache.try_get(material->texDirSpecular(), speculaTex);

    if(!ambientTex)
    {
      ambientTex =  device->createTextureFromImage(m_ambientImage[sm->getMaterialKey()], Converters::convertWrapMode(material->mappingModeAmbient()));
      textureCache.insert(material->texDirAmbient(), ambientTex);
    }
    if(!diffuseTex)
    {
      diffuseTex =  device->createTextureFromImage(m_diffuseImage[sm->getMaterialKey()], Converters::convertWrapMode(material->mappingModeDiffuse()));
      textureCache.insert(material->texDirDiffuse(), diffuseTex);
    }
    if(!speculaTex)
    {
      speculaTex =  device->createTextureFromImage(m_specularImage[sm->getMaterialKey()], Converters::convertWrapMode(material->mappingModeSpecular()));
      textureCache.insert(material->texDirDiffuse(), speculaTex);
    }

    sa::VertexBufferPtr vb = device->createVertexBuffer(&sm->getVertices().front(), sm->getVertices().size() * sizeof(SubMeshModel::Vertex));
    sa::VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);
    sa::IndexBufferPtr ib = device->createIndexBuffer(sm->getIndices());

    subMeshDrawData.IsTwoSided = material->isTwoSided();
    subMeshDrawData.VAO = vao;
    subMeshDrawData.IB = ib;
    subMeshDrawData.SP = sp;
    subMeshDrawData.SSP = ssp;
    subMeshDrawData.TEX[0] = ambientTex;
    subMeshDrawData.TEX[1] = diffuseTex;
    subMeshDrawData.TEX[2] = speculaTex;

    if(ambientTex) {
      subMeshDrawData.Sampler2DUniforms[sp->uniformLocation("u_ambientTexture")] = 0;
    }
    else {
      subMeshDrawData.Vec4Uniforms[sp->uniformLocation("u_ambientMaterial")] = material->ambient();
    }
    if(diffuseTex) {
      subMeshDrawData.Sampler2DUniforms[sp->uniformLocation("u_diffuseTexture")] = 1;
    }
    else {
      subMeshDrawData.Vec4Uniforms[sp->uniformLocation("u_diffuseMaterial")] = material->diffuse();
    }
    if(speculaTex) {
      subMeshDrawData.Sampler2DUniforms[sp->uniformLocation("u_specularTexture")] = 2;
    }
    else {
      subMeshDrawData.Vec4Uniforms[sp->uniformLocation("u_specularMaterial")] = material->specular();
    }
    subMeshDrawData.FloatUniforms[sp->uniformLocation("u_shininess")] = material->shininess();
    subMeshDrawData.FloatUniforms[sp->uniformLocation("u_shininessStrength")] = material->shininessStrength();

    subMeshDrawData.Matrix4Uniforms[sp->uniformLocation("u_modelMatrix")] = sa::Matrix44T<float>::GetIdentity();
    m_drawData[meshIndex] = subMeshDrawData;
    ++meshIndex;
  }
  //m_currentDataStorage = DataStorage::GPU;
  context->resetCurrentState();

  //Apply the transformations at least once. If the nodes are animated we will update every frame.
  if(m_meshModel.m_data.m_transformationTreeRoot)
  {
    VisitorApplyTransformations visitor;
    m_meshModel.m_data.m_transformationTreeRoot->accept(&visitor);
  }

}

void MeshRenderable::unloadGPU() {
  m_drawData.clear();
  m_drawDataDeque.clear();
}

void MeshRenderable::unloadCPU() {
  m_meshModel.unload();
}

//void MeshRenderable::update(float currentTime) {
//  //if(m_currentDataStorage == DataStorage::GPU) {
//    applyAnimations(currentTime);
//    applyTransformations();
//  //}
//}

void MeshRenderable::applyAnimations(float dt) {
  //dt = 0;

  if(m_meshModel.m_data.m_animations.size() > 0) {
    MeshModel::Data::Animations::const_iterator nodeAnimIt =  m_meshModel.m_data.m_animations.find(m_currentNodeAnimation);
    if(m_meshModel.m_data.m_animations.end() != nodeAnimIt) {
      VisitorApplyAnimation* visitor = new VisitorApplyAnimation(m_currentNodeAnimationTime, (*nodeAnimIt).second);
      m_meshModel.m_data.m_transformationTreeRoot->accept(visitor);
      delete visitor;
    }
  }


  for(sa::MeshModel::Data::SubMeshes::value_type subMesh : m_meshModel.m_data.m_subMeshes) {
    if(subMesh->skeleton() && subMesh->skeleton()->Animations.end() != subMesh->skeleton()->Animations.find(m_currentSkeletalAnimation)) {
      subMesh->skeleton()->animate(m_currentSkeletalAnimationTime, m_currentSkeletalAnimation);
    }
  }
  m_currentSkeletalAnimationTime+=dt;
  m_currentNodeAnimationTime+=dt;
}

void MeshRenderable::applyTransformations() {
  if(m_meshModel.m_data.m_haveBones) {
    //////////////////////////////////////////////////
    //Apply the skeleton.
    //////////////////////////////////////////////////
    unsigned int subMeshIndex = 0;
    for(sa::MeshModel::Data::SubMeshes::value_type subMesh : m_meshModel.m_data.m_subMeshes) {
      Skeleton* skeleton = subMesh->skeleton();
      skeleton->applyTransformations();
      unsigned int i = 0;
      for(Skeleton::JointMap::value_type j : skeleton->Joints) {
        int uniLoc = m_drawData[subMeshIndex].SP->uniformLocation("u_bones");
        m_drawData[subMeshIndex].Matrix4Uniforms[uniLoc+i] = j.second.Transformation;
        i++;
      }
      ++subMeshIndex;
    }
  }

  m_drawDataDeque.clear();
  if(m_meshModel.m_data.m_transformationTreeRoot && m_meshModel.m_data.m_animations.size() > 0)
  {
    VisitorApplyTransformations visitor;
    m_meshModel.m_data.m_transformationTreeRoot->accept(&visitor);
  }
  if(m_meshModel.m_data.m_transformationTreeRoot)
  {
    const std::set<MeshNodeModel*>& meshNodes = m_meshModel.getMeshNodes();
    for(MeshNodeModel* mesh : meshNodes) {
      //m_drawData[mesh->mesh()].Matrix4Uniforms[m_drawData[mesh->mesh()].SP->uniformLocation("u_modelMatrix")] =  mesh->transformation();
      m_drawData[mesh->mesh()].Matrix4Uniforms[m_modelMatrixUniform] =  mesh->transformation();
      m_drawDataDeque.push_back(m_drawData[mesh->mesh()]);
    }
  }
}

std::deque<std::string> MeshRenderable::getSkeletalAnimations() const {
  return m_meshModel.m_header.skeletalAnimations;
}

void MeshRenderable::playSkeletalAnimation(const std::string& animationName) {
  m_currentSkeletalAnimationTime = 0.0f;
  m_currentSkeletalAnimation = animationName;
}

std::deque<std::string> MeshRenderable::getNodeAnimations() const {
  return m_meshModel.m_header.nodeAnimations;
}

void MeshRenderable::playNodeAnimation(const std::string& animationName) {
  m_currentNodeAnimationTime = 0.0f;
  m_currentNodeAnimation = animationName;
}
}

