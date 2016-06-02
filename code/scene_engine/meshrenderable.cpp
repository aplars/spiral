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

void MeshRenderable::toCPU(ImageCache& imageCache, const std::string& shaderPath) {
  if(m_currentDataStorage != DataStorage::Disk)
    return;

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
  }

  m_vshCode = RenderDevice::readFromFile((shaderPath + "ubershader.vsh").c_str()),
  m_fshCode = RenderDevice::readFromFile((shaderPath + "ubershader.fsh").c_str());
  m_spKey = m_vshCode + m_fshCode;

  m_vshShadowCode = RenderDevice::readFromFile((shaderPath + "ubershadowshader.vsh").c_str()),
  m_fshShadowCode = RenderDevice::readFromFile((shaderPath + "ubershadowshader.fsh").c_str());
  m_spShadowKey = m_vshShadowCode + m_fshShadowCode;

  m_currentDataStorage = DataStorage::CPU;
}

std::set<std::string> getShaderDefinesFromMesh(const MeshModel& mesh) {
  std::set<std::string> defines;
  if(mesh.m_data.m_haveBones) {
    defines.insert("BONE_ANIMATION");
  }
  return defines;
}

std::set<std::string> getShaderDefinesFromSubMesh(const SubMeshModel& subMesh, const MeshModel::Data::Materials& materials) {
  std::set<std::string> defines;
  auto material = materials[subMesh.getMaterialKey()];
  if(!material->texDirAmbient().empty()) {
    defines.insert("AMBIENT_TEXTURE");
  }
  if(!material->texDirDiffuse().empty()) {
    defines.insert("DIFFUSE_TEXTURE");
  }
  if(!material->texDirSpecular().empty()) {
    defines.insert("SPECULAR_TEXTURE");
  }
  return defines;
}

ShaderProgramPtr createShaderProgramFromFile(const std::set<std::string>& defines, const std::string& vshDir, const std::string& fshDir, ShaderCache& cache, RenderDevice* device) {
  ShaderProgramPtr sp;
  std::string definesAsString;
  for(auto define : defines) {
    definesAsString+=define;
  }
  auto shaderkey = definesAsString + vshDir + fshDir;
  if(!cache.try_get(shaderkey, sp)) {
    sp = device->createShaderProgramFromFile(
          vshDir.c_str(),
          fshDir.c_str(),
          defines);
    cache.insert(shaderkey, sp);
  }
  return sp;
}

ShaderProgramPtr createShaderProgramFromCode(const std::string& key, const std::set<std::string>& defines, const std::string& vsh, const std::string& fsh, ShaderCache& cache, RenderDevice* device) {
  ShaderProgramPtr sp;
  std::string definesAsString;
  for(auto define : defines) {
    definesAsString+=define;
  }
  auto shaderkey = definesAsString + key;
  if(!cache.try_get(shaderkey, sp)) {
    sp = device->createShaderProgram(
          vsh.c_str(),
          fsh.c_str(),
          defines);
    cache.insert(shaderkey, sp);
  }
  return sp;
}

void set_insert_range(const std::set<std::string>& in, std::set<std::string>& out) {
  out.insert(in.begin(), in.end());
}

void MeshRenderable::toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context) {
  if(m_currentDataStorage == DataStorage::GPU) {
    m_numberOfInstances++;

    return;
  }
  if(m_currentDataStorage != DataStorage::CPU)
    return;

  std::set<std::string> globalDefines;

  globalDefines.insert("NUMBER_OF_CASCADES " + std::to_string(numberOfShadowCascades));
  set_insert_range(getShaderDefinesFromMesh(m_meshModel), globalDefines);

  unsigned int meshIndex = 0;
  for(sa::MeshModel::Data::SubMeshes::value_type sm : m_meshModel.m_data.m_subMeshes) {
    std::set<std::string> defines;
    std::set<std::string> subMeshDefines = getShaderDefinesFromSubMesh(*sm, m_meshModel.m_data.m_materials);
    set_insert_range(globalDefines, defines);
    set_insert_range(subMeshDefines, defines);

    ShaderProgramPtr sp = createShaderProgramFromCode(
          m_spKey,
          defines,
          m_vshCode.c_str(),
          m_fshCode.c_str(),
          shaderCache,
          device);

    ShaderProgramPtr ssp = createShaderProgramFromCode(
          m_spShadowKey,
          defines,
          m_vshShadowCode,
          m_fshShadowCode,
          shaderCache,
          device);

    int posAttr = sp->attributeLocation("posAttr");
    int norAttr = sp->attributeLocation("norAttr");
    int texAttr = sp->attributeLocation("texAttr");
    int bAttr = sp->attributeLocation("bAttr");
    int wAttr = sp->attributeLocation("wAttr");

    ssp->bindAttributeLocation("posAttr", posAttr);
    ssp->bindAttributeLocation("norAttr", norAttr);
    ssp->bindAttributeLocation("bAttr", bAttr);
    ssp->bindAttributeLocation("wAttr", wAttr);
    ssp->link();

    sa::VertexDescription vertexDesc =
    {
      {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
      {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
      {norAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
      {bAttr, sa::VertexDescriptionElement::Type::FLOAT, 4},
      {wAttr, sa::VertexDescriptionElement::Type::FLOAT, 4}
    };


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
      subMeshDrawData.Uniforms.Sampler2DUniforms["u_ambientTexture"] = 0;
    }
    if(diffuseTex) {
      subMeshDrawData.Uniforms.Sampler2DUniforms["u_diffuseTexture"] = 1;
    }
    //else {
      subMeshDrawData.Uniforms.Vec4Uniforms["u_diffuseMaterial"] = material->diffuse();
    //}
    if(speculaTex) {
      subMeshDrawData.Uniforms.Sampler2DUniforms["u_specularTexture"] = 2;
    }
    else {
      subMeshDrawData.Uniforms.Vec4Uniforms["u_specularMaterial"] = material->specular();
    }
    subMeshDrawData.Uniforms.FloatUniforms["u_shininess"] = material->shininess();
    subMeshDrawData.Uniforms.FloatUniforms["u_shininessStrength"] = material->shininessStrength();

    subMeshDrawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = sa::Matrix44T<float>::GetIdentity();
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
  if(m_meshModel.m_data.m_transformationTreeRoot)
  {
    const std::set<MeshNodeModel*>& meshNodes = m_meshModel.getMeshNodes();
    for(MeshNodeModel* mesh : meshNodes) {
      m_drawData[mesh->mesh()].Uniforms.Matrix4Uniforms["u_modelMatrix"] =  mesh->transformation();
      m_drawDataDeque.push_back(m_drawData[mesh->mesh()]);
    }
  }
  m_numberOfInstances++;
  m_currentDataStorage = DataStorage::GPU;
}

void MeshRenderable::unload() {
  if(m_numberOfInstances <= 1) {
    m_currentDataStorage = DataStorage::Pending;
    m_drawData.clear();
    m_drawDataDeque.clear();
    m_meshModel.unload();
    m_currentDataStorage = DataStorage::Disk;
    qDebug() << "MeshRenderable::unload()";
  }
  if(m_numberOfInstances > 0)
    m_numberOfInstances--;
}

const sa::DrawDataList& MeshRenderable::getDrawData() const
{
  return m_drawDataDeque;
}

sa::DrawDataList& MeshRenderable::getDrawData()
{
  return m_drawDataDeque;
}

sa::DrawData MeshRenderable::getDrawData(unsigned int subMesh)
{
  return m_drawData[subMesh];
}


void MeshRenderable::setAnimationFrame(const std::string& skeletalAnimationName, const std::string& nodeAnimationName, float currentSkeletalAnimationTime, float currentNodeAnimationTime) {

  if(m_meshModel.m_data.m_animations.size() > 0) {
    for(MeshModel::Data::Animations::value_type anim : m_meshModel.m_data.m_animations) {
      VisitorApplyAnimation* visitor = new VisitorApplyAnimation(0, anim.second);
      m_meshModel.m_data.m_transformationTreeRoot->accept(visitor);
      delete visitor;

    }

    MeshModel::Data::Animations::const_iterator nodeAnimIt =  m_meshModel.m_data.m_animations.find(nodeAnimationName);
    if(m_meshModel.m_data.m_animations.end() != nodeAnimIt) {
      VisitorApplyAnimation* visitor = new VisitorApplyAnimation(currentNodeAnimationTime, (*nodeAnimIt).second);
      m_meshModel.m_data.m_transformationTreeRoot->accept(visitor);
      delete visitor;
    }
  }

  for(sa::MeshModel::Data::SubMeshes::value_type subMesh : m_meshModel.m_data.m_subMeshes) {
    if(subMesh && subMesh->skeleton())
      subMesh->skeleton()->reset();
    if(subMesh && subMesh->skeleton() && subMesh->skeleton()->Animations.end() != subMesh->skeleton()->Animations.find(skeletalAnimationName)) {
      subMesh->skeleton()->animate(currentSkeletalAnimationTime, skeletalAnimationName);
    }
  }

  //applyTransformations();
}


void MeshRenderable::applyTransformations() {
  if(m_currentDataStorage != DataStorage::GPU)
    return;
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
        std::string uniformName = std::string("u_bones") + std::string("[") + std::to_string(i) + std::string("]");
        m_drawData[subMeshIndex].Uniforms.Matrix4Uniforms[uniformName] = j.second.Transformation;
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
  if(m_meshModel.m_data.m_transformationTreeRoot /*&& m_meshModel.m_data.m_animations.size() > 0*/)
  {
    const std::set<MeshNodeModel*>& meshNodes = m_meshModel.getMeshNodes();
    int i = 0;

    for(MeshNodeModel* mesh : meshNodes) {
      m_drawData[mesh->mesh()].Uniforms.Matrix4Uniforms["u_modelMatrix"] =  mesh->transformation();
      m_drawDataDeque.push_back(m_drawData[mesh->mesh()]);
      i++;
    }
  }
}

std::deque<std::string> MeshRenderable::getSkeletalAnimations() const {
  return m_meshModel.m_header.skeletalAnimations;
}


std::deque<std::string> MeshRenderable::getNodeAnimations() const {
  return m_meshModel.m_header.nodeAnimations;
}

}

