#include "assimptosamodels.h"
#include <QDebug>
#include <fstream>
#include <limits>
#include <boost/serialization/deque.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/uuid/uuid_generators.hpp>
#include <assimp/matrix4x4.h>
#include <scene_models/meshmodel.h>
#include <scene_models/skeletonmodel.h>
#include <scene_models/meshnodemodel.h>
#include <scene_models/animationchannelmodel.h>
#include <scene_models/animationmodel.h>
#include <scene_models/visitorgettransformationnode.h>
#include <math/mat4ext.h>
BOOST_CLASS_EXPORT(sa::MeshNodeModel)
BOOST_CLASS_EXPORT(sa::TransformationNodeModel)

bool operator<(const aiString& a, const aiString& b) {
  return (strcmp(a.C_Str(), b.C_Str()) < 0);
  //return strcmp(a.)
}

static unsigned int ppsteps =

    //aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
    aiProcess_JoinIdenticalVertices    | // join identical vertices/ optimize indexing
    aiProcess_ValidateDataStructure    | // perform a full validation of the loader's output
    aiProcess_ImproveCacheLocality     | // improve the cache locality of the output vertices
   // aiProcess_RemoveRedundantMaterials | // remove redundant materials
    aiProcess_FindDegenerates          | // remove degenerated polygons from the import
    aiProcess_FindInvalidData          | // detect invalid model data, such as invalid normal vectors
    aiProcess_GenUVCoords              | // convert spherical, cylindrical, box and planar mapping to proper UVs
    aiProcess_TransformUVCoords        | // preprocess UV transformations (scaling, translation ...)
    aiProcess_FindInstances            | // search for instanced meshes and remove them by references to one master
    aiProcess_LimitBoneWeights         | // limit bone weights to 4 per vertex
    aiProcess_OptimizeMeshes		   | // join small meshes, if possible;
    aiProcess_SplitByBoneCount         | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
  0;


//static const unsigned int ppsteps = aiProcess_PreTransformVertices|
//                                       aiProcess_CalcTangentSpace|
//                                       aiProcess_GenSmoothNormals|
//                                       aiProcess_Triangulate|
//                                       aiProcess_FixInfacingNormals|
//                                       aiProcess_FindInvalidData | aiProcess_LimitBoneWeights |
//                                       aiProcess_ValidateDataStructure | 0;

bool AssimpToSAModels::convertToXML(bool toXML, const QDir& sourceDir, const QDir& destDir, bool isStatic, float scaleFactor)
{
  bool isOk = true;
  float g_smoothAngle = 80.f;
  bool nopointslines = false;

  aiPropertyStore* props = aiCreatePropertyStore();
  aiSetImportPropertyInteger(props,AI_CONFIG_IMPORT_TER_MAKE_UVS,1);
  aiSetImportPropertyFloat(props,AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE,g_smoothAngle);
  aiSetImportPropertyInteger(props,AI_CONFIG_PP_SBP_REMOVE,nopointslines ? aiPrimitiveType_LINE | aiPrimitiveType_POINT : 0 );
  aiSetImportPropertyInteger(props,AI_CONFIG_GLOB_MEASURE_TIME,1);

  std::string sourceDirString = sourceDir.absolutePath().toStdString();
  if(isStatic)
    ppsteps = ppsteps | aiProcess_PreTransformVertices;
  const aiScene* scene = (aiScene*)aiImportFileExWithProperties(
        sourceDirString.c_str(),
        ppsteps | /* configurable pp steps */
        aiProcess_GenSmoothNormals		   | // generate smooth normal vectors if not existing
        //aiProcess_SplitLargeMeshes         | // split large, unrenderable meshes into submeshes
        aiProcess_Triangulate			   | // triangulate polygons with more than 3 edges
        //aiProcess_ConvertToLeftHanded	   | // convert everything to D3D left handed space
        aiProcess_SortByPType              | // make 'clean' meshes which consist of a single typ of primitives
        0,
        NULL,
        props);
  if(!scene) {
    isOk = false;
  }
  if(isOk) {
    if (scaleFactor != 1.0f)
    {
      aiMatrix4x4 scaling;
      aiMatrix4x4::Scaling( aiVector3D(scaleFactor), scaling );
      const_cast<aiMatrix4x4&>(scene->mRootNode->mTransformation) = scaling * scene->mRootNode->mTransformation;
    }

    std::deque<sa::MaterialModel*> saMaterials = processMaterials(scene);
    std::deque<sa::SubMeshModel*> saMeshes = processMeshes(scene);
    std::map<std::string, boost::uuids::uuid> nodeKeys;
    unsigned int numMeshesInSubTree = 0;
    sa::NodeModel* transformationRoot = processTransformationTree(scene->mRootNode, nodeKeys, numMeshesInSubTree);
    std::deque<sa::AnimationModel<boost::uuids::uuid> * > animations = processAnimations(scene, nodeKeys);
    bool haveBones = processBones(scene, saMeshes, nodeKeys);
    //Assimp not needed anymore. Remove it
    delete scene;
    scene = nullptr;
    aiReleasePropertyStore(props);

    //Copy the textures to the same directory as the mesh data.
    for(const sa::MaterialModel* mat : saMaterials) {
      QDir srccpy = sourceDir;
      srccpy.cdUp();
      srccpy=QDir(srccpy.absolutePath() + QDir::separator() + QString(mat->texDirDiffuse().c_str()));
      QDir dstcpy = destDir;
      dstcpy.cdUp();


      //create folder if needed.
      QFileInfo fi =  QFileInfo(dstcpy.absolutePath() + QDir::separator() + QString(mat->texDirDiffuse().c_str()));


      QDir mejkdir(fi.absoluteDir());
      if(!mejkdir.exists()) {
        //mejkdir.mkdir(".");
        QDir::home().mkdir(fi.absoluteDir().absolutePath()); //make dir
      }

      dstcpy=QDir(dstcpy.absolutePath() + QDir::separator() + QString(mat->texDirDiffuse().c_str()));


      qDebug() << "copy " << srccpy.absolutePath() << " -- " << dstcpy.absolutePath();
      if(!QFile::copy(srccpy.absolutePath(), dstcpy.absolutePath())) {
        QDir srccpy = sourceDir;
        srccpy.cdUp();
        QString difflow = QString(mat->texDirDiffuse().c_str()).toLower();
        srccpy=QDir(srccpy.absolutePath() + QDir::separator() + difflow);
        QDir dstcpy = destDir;
        dstcpy.cdUp();
        dstcpy=QDir(dstcpy.absolutePath() + QDir::separator() + QString(mat->texDirDiffuse().c_str()));
        QFile::copy(srccpy.absolutePath(), dstcpy.absolutePath());
      }
    }



    //Create a spiral architect scene and add the resources to it.
    sa::MeshModel saModel;
    saModel.m_header.boundingBox = calculateBoundingBox(haveBones, saMeshes, animations, transformationRoot);
    saModel.setHaveBones(haveBones);
    saModel.setMaterials(saMaterials);
    saModel.setMeshes(saMeshes);
    saModel.setTransformationTree(transformationRoot);
    saModel.setAnimations(animations);
    saModel.m_header.skeletalAnimations = saModel.getSkeletalAnimations();
    saModel.m_header.nodeAnimations = saModel.getNodeAnimations();




    //Save the header
    {
      std::string destDirString = (destDir.absolutePath().toStdString())+".header";
      std::ofstream ofs(destDirString.c_str(), std::ios_base::binary);
      if(ofs.good())
      {
        if(toXML) {
          boost::archive::xml_oarchive oa(ofs);
          oa << BOOST_SERIALIZATION_NVP(saModel.m_header);
        }
        else {
          boost::archive::binary_oarchive oa(ofs);
          oa << BOOST_SERIALIZATION_NVP(saModel.m_header);
        }
      }
      else {
        isOk = false;
      }
    }
    //Save the data
    {
      std::string destDirString = (destDir.absolutePath().toStdString())+".data";
      std::ofstream ofs(destDirString.c_str(), std::ios_base::binary);
      if(ofs.good())
      {
        if(toXML) {
          boost::archive::xml_oarchive oa(ofs);
          oa << BOOST_SERIALIZATION_NVP(saModel.m_data);
        }
        else {
          boost::archive::binary_oarchive oa(ofs);
          oa << BOOST_SERIALIZATION_NVP(saModel.m_data);
        }
      }
      else {
        isOk = false;
      }
    }

  }
  return isOk;
}

std::deque<sa::MaterialModel*> AssimpToSAModels::processMaterials(const aiScene* const scene) {
  std::deque<sa::MaterialModel*> saMaterials;
  for(unsigned int materialIt = 0; materialIt < scene->mNumMaterials; ++materialIt) {
    const aiMaterial* material = scene->mMaterials[materialIt];

    aiColor4D diffuseColor, ambientColor, specularColor;
    float shininess, shininessStrength;

    aiBlendMode blendMode;
    GetMaterialProperty(material, AI_MATKEY_BLEND_FUNC, aiBlendMode_Default, blendMode);

    GetMaterialProperty(material, AI_MATKEY_SHININESS, 15.0f, shininess);
    GetMaterialProperty(material, AI_MATKEY_SHININESS_STRENGTH, 1.0f, shininessStrength);
    GetMaterialProperty(material, AI_MATKEY_COLOR_AMBIENT, aiColor4D(0.0, 0.0, 0.0, 1.0), ambientColor);
    GetMaterialProperty(material, AI_MATKEY_COLOR_DIFFUSE, aiColor4D(0.0, 0.0, 0.0, 1.0), diffuseColor);
    GetMaterialProperty(material, AI_MATKEY_COLOR_SPECULAR, aiColor4D(0.0, 0.0, 0.0, 1.0), specularColor);
    aiTextureMapMode mappingModeAmbient, mappingModeDiffuse, mappingModeSpecular;
    aiTextureMapMode mappingModeVAmbient, mappingModeVDiffuse, mappingModeVSpecular;
    aiString texDirAmbient, texDirDiffuse, texDirSpecular, texDirNormals;
    material->Get(AI_MATKEY_TEXTURE_AMBIENT(0), texDirAmbient);
    material->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), texDirDiffuse);
    material->Get(AI_MATKEY_TEXTURE_SPECULAR(0), texDirSpecular);
    material->Get(AI_MATKEY_TEXTURE_HEIGHT(0), texDirNormals);

    GetMaterialProperty(material, AI_MATKEY_MAPPINGMODE_U_AMBIENT(0), aiTextureMapMode_Wrap, mappingModeAmbient);
    GetMaterialProperty(material, AI_MATKEY_MAPPINGMODE_U_DIFFUSE(0), aiTextureMapMode_Wrap, mappingModeDiffuse);
    GetMaterialProperty(material, AI_MATKEY_MAPPINGMODE_U_SPECULAR(0), aiTextureMapMode_Wrap, mappingModeSpecular);

    GetMaterialProperty(material, AI_MATKEY_MAPPINGMODE_V_AMBIENT(0), aiTextureMapMode_Wrap, mappingModeVAmbient);
    GetMaterialProperty(material, AI_MATKEY_MAPPINGMODE_V_DIFFUSE(0), aiTextureMapMode_Wrap, mappingModeVDiffuse);
    GetMaterialProperty(material, AI_MATKEY_MAPPINGMODE_V_SPECULAR(0), aiTextureMapMode_Wrap, mappingModeVSpecular);


    int twoSided;
    GetMaterialProperty(material, AI_MATKEY_TWOSIDED, 1, twoSided);

    float opacity = 1.0f;
    GetMaterialProperty(material, AI_MATKEY_OPACITY, 1.0f, opacity);

    sa::MaterialModel* saMaterial = new sa::MaterialModel(
          static_cast<bool>(twoSided),
          getSaBlendModeFromAi(blendMode),
          texDirAmbient.C_Str(),
          texDirDiffuse.C_Str(),
          texDirSpecular.C_Str(),
          getSaTexMapModeFromAi(mappingModeAmbient),
          getSaTexMapModeFromAi(mappingModeDiffuse),
          getSaTexMapModeFromAi(mappingModeSpecular),
          getSaColorFromAi(ambientColor),
          getSaColorFromAi(diffuseColor),
          getSaColorFromAi(specularColor),
          shininess,
          shininessStrength);
    saMaterials.push_back(saMaterial);
  }
  return saMaterials;
}


std::deque<sa::SubMeshModel*> AssimpToSAModels::processMeshes(
    const aiScene* const scene) {

  std::deque<sa::SubMeshModel*> saMeshes;
  for(unsigned int meshIt = 0; meshIt < scene->mNumMeshes; ++meshIt) {
    const aiMesh* const mesh = scene->mMeshes[meshIt];
    qDebug() << "mesh name: " << mesh->mName.C_Str();
    sa::SubMeshModel* saMesh = new sa::SubMeshModel(mesh->mName.C_Str());
    //Procsses the vertices and adds them to the model.
    for(unsigned int vertexIt=0; vertexIt < mesh->mNumVertices; ++vertexIt) {
      aiVector3D vertex = mesh->mVertices[vertexIt];
      aiVector3D tex;

      if(mesh->GetNumUVChannels() > 0 && mesh->mNumUVComponents[0] > 0)
        tex = mesh->mTextureCoords[0][vertexIt];
      aiVector3D norm;
      if(mesh->mNormals)
        norm = mesh->mNormals[vertexIt];

      saMesh->addVertex(sa::SubMeshModel::Vertex(vertex.x, vertex.y, vertex.z, tex.x, tex.y, norm.x, norm.y, norm.z));
    }

    //Procsses the faces and adds them to the model.
    for(unsigned int faceIt = 0; faceIt < mesh->mNumFaces; ++faceIt) {
      const aiFace face = mesh->mFaces[faceIt];
      if(face.mNumIndices == 3) {
        unsigned int a = face.mIndices[0];
        unsigned int b = face.mIndices[1];
        unsigned int c = face.mIndices[2];
        saMesh->addFace(sa::SubMeshModel::Face(a, b, c));
      }
    }

    //std::map<unsigned int, boost::uuids::uuid>::const_iterator it = materialKeys.find(mesh->mMaterialIndex);
    saMesh->setMaterialKey(mesh->mMaterialIndex);
//    if(it != materialKeys.end()) {
//      saMesh->setMaterialKey((*it).second);
//    }
    saMeshes.push_back(saMesh);
  }
  return saMeshes;
}


sa::NodeModel* AssimpToSAModels::processTransformationTree(const aiNode* const currentNode,
                                                           std::map<std::string, boost::uuids::uuid>& nodeKeys, unsigned int &numMeshes) {
  sa::TransformationNodeModel* saNode = new sa::TransformationNodeModel(currentNode->mName.C_Str(), getSaMatrixFromAi(currentNode->mTransformation));
  nodeKeys[std::string(currentNode->mName.C_Str())] = saNode->getKey();
  for(unsigned int meshIt = 0; meshIt < currentNode->mNumMeshes; ++meshIt) {
    unsigned int meshIndex = currentNode->mMeshes[meshIt];
    saNode->addChild(new sa::MeshNodeModel(meshIndex));
    ++numMeshes;
  }

  for(unsigned int childIt = 0; childIt < currentNode->mNumChildren; ++childIt) {
    unsigned int numMeshesInSubNode = 0;
    aiNode* child = currentNode->mChildren[childIt];
    sa::NodeModel* saChild = processTransformationTree(child, nodeKeys, numMeshesInSubNode);
    numMeshes += numMeshesInSubNode;
    //Only add nodes that need transformation. That is, nodes with meshes to transform.
    if(numMeshesInSubNode > 0) {
      saNode->addChild(saChild);
    }
    else {
      nodeKeys.erase(nodeKeys.find(saChild->getName()));
      delete saChild;
      saChild = nullptr;
    }
  }
  return saNode;
}

std::deque<sa::AnimationModel<boost::uuids::uuid> * > AssimpToSAModels::processAnimations(const aiScene* const scene, const std::map<std::string, boost::uuids::uuid>& nodeKeys) {
  std::deque<sa::AnimationModel<boost::uuids::uuid> * > saAnimations;
  for(unsigned int animIt = 0; animIt < scene->mNumAnimations; ++animIt) {
    sa::AnimationModel<boost::uuids::uuid > * saAnimation = new sa::AnimationModel<boost::uuids::uuid > ();
    const aiAnimation* const anim = scene->mAnimations[animIt];
    saAnimation->Name = std::string(anim->mName.C_Str());

    float ticksPerSec = anim->mTicksPerSecond;
    if(ticksPerSec <= 0.0001)
      ticksPerSec = 60.0f;

    saAnimation->Duration = static_cast<float>(anim->mDuration/ticksPerSec);
    for(unsigned int channelIt = 0; channelIt < anim->mNumChannels; ++channelIt) {
      sa::AnimationChannelModel saChannel;
      const aiNodeAnim* const nodeAnim = anim->mChannels[channelIt];

      for(unsigned int positionIt = 0; positionIt < nodeAnim->mNumPositionKeys; ++positionIt) {
        saChannel.Translation.push_back(sa::AnimationChannelModel::TranslationKey(nodeAnim->mPositionKeys[positionIt].mTime/ticksPerSec, getGlmVector3FromAi(nodeAnim->mPositionKeys[positionIt].mValue)));
      }
      for(unsigned int rotationIt = 0; rotationIt < nodeAnim->mNumRotationKeys; ++rotationIt) {
        saChannel.Quaternion.push_back(sa::AnimationChannelModel::QuaternionKey(nodeAnim->mRotationKeys[rotationIt].mTime/ticksPerSec, getGlmQuatFromAi(nodeAnim->mRotationKeys[rotationIt].mValue)));
      }
      std::string nodeName = std::string(nodeAnim->mNodeName.C_Str());
      std::map<std::string, boost::uuids::uuid>::const_iterator findIt = nodeKeys.find(nodeName);
      if(findIt != nodeKeys.end())
        saAnimation->Channels[findIt->second] = saChannel;
    }
    if(saAnimation->Channels.size() > 0)
      saAnimations.push_back(saAnimation);
  }
  return saAnimations;
}

bool AssimpToSAModels::processBones(
    const aiScene* const scene,
    std::deque<sa::SubMeshModel*> saMeshes,
    const std::map<std::string, boost::uuids::uuid>& nodeKeys)
{
  bool haveBones = false;
  for(std::map<std::string, boost::uuids::uuid>::value_type saNodes : nodeKeys) {
    aiNode* node = scene->mRootNode->FindNode(saNodes.first.c_str());
    if(node->mNumMeshes > 0) {
      std::deque<sa::Skeleton*> skeletons = processSkeletonsForMeshNode(scene, node);
      for(unsigned int i = 0; i < node->mNumMeshes; ++i) {
        if(skeletons[i]->Joints.size() <= 0)
          continue;
        sa::SubMeshModel* saSubMesh = saMeshes[node->mMeshes[i]];
        saSubMesh->setSkeleton(skeletons[i]);
        sa::Skeleton* saSkeleton = skeletons[i];

        int boneId = 0;
        for(sa::Skeleton::JointMap::value_type jointEntry : saSkeleton->Joints) {
          sa::JointModel joint = jointEntry.second;
          for(sa::JointModel::VertexWeight weight : joint.Weights) {
            saSubMesh->setBoneDataForVertex(weight.VertexIndex, boneId, weight.Weight);
          }
          ++boneId;
        }

        haveBones = true;
      }
    }
  }
  return haveBones;
}

std::deque<sa::Skeleton*> AssimpToSAModels::processSkeletonsForMeshNode(const aiScene* scene, aiNode* const meshNode)
{
  std::deque<sa::Skeleton*> skeletons;
  for(unsigned int i = 0; i < meshNode->mNumMeshes; ++i)
  {
    aiMesh* mesh = scene->mMeshes[meshNode->mMeshes[i]];

    std::map<aiNode* const, const aiBone*> boneNodes = getBoneNodes(scene, meshNode, mesh);
    typedef std::map<aiNode* const, const aiBone*>::value_type map_value_type;

    std::map<aiNode*, std::string> NodeToSAJointIndex;

    sa::Skeleton* skeleton = new sa::Skeleton();
    for(map_value_type b : boneNodes)
    {

      if(b.second)
      {
        std::vector<sa::JointModel::VertexWeight> weights;
        for(unsigned int j = 0; j < b.second->mNumWeights; ++j)
        {
          weights.push_back(sa::JointModel::VertexWeight(b.second->mWeights[j].mVertexId, b.second->mWeights[j].mWeight));
        }


        skeleton->Joints[b.first->mName.data] = (sa::JointModel(b.first->mName.data, getSaMatrixFromAi(b.second->mOffsetMatrix), getSaMatrixFromAi(b.first->mTransformation), weights));
        NodeToSAJointIndex[b.first] = b.first->mName.data;
      }
      else
      {
        std::vector<sa::JointModel::VertexWeight> weights;

        skeleton->Joints[b.first->mName.data] = (sa::JointModel(b.first->mName.data, glm::mat4(1.0f), getSaMatrixFromAi(b.first->mTransformation), weights));
        NodeToSAJointIndex[b.first] = b.first->mName.data;
      }
    }
    //Create the SA joint hierarcy.
    for(map_value_type b : boneNodes)
    {
      if(b.first->mParent)
      {
        std::string cur = NodeToSAJointIndex[b.first];
        std::map<aiNode*, std::string>::const_iterator it =  NodeToSAJointIndex.find(b.first->mParent);
        if(it != NodeToSAJointIndex.end())
        {
          skeleton->Joints[cur].Parent = it->second;


          skeleton->Joints[skeleton->Joints[cur].Parent].Children.insert(cur);
        }
        else
        {
          skeleton->Root = cur;
        }
      }
    }
    processSkeletalAnimations(scene, skeleton);
    skeletons.push_back(skeleton);
  }
  return skeletons;
}

std::map<aiNode* const, const aiBone*> AssimpToSAModels::getBoneNodes(const aiScene* scene, aiNode* const meshNode, aiMesh* const mesh) {
  std::map<aiNode* const, const aiBone*> boneNodes;

  for(unsigned int i = 0; i < mesh->mNumBones; ++i)
  {
    aiNode* boneNode = scene->mRootNode->FindNode(mesh->mBones[i]->mName);
    boneNodes[boneNode] = mesh->mBones[i];
  }

  for(unsigned int i = 0; i < mesh->mNumBones; ++i)
  {
    aiNode* boneNode = scene->mRootNode->FindNode(mesh->mBones[i]->mName);
    boneNodes[boneNode] = mesh->mBones[i];

    //Traverse up to mesh node.
    while(boneNode != meshNode && boneNode != meshNode->mParent)
    {
      if(boneNodes.find(boneNode) == boneNodes.end())
      {
        aiBone* identityBone = new aiBone();
        identityBone->mName = "SPIRAL_ARCHITECT_IDENTITY_BONE";
        identityBone->mNumWeights = 0;

        boneNodes[boneNode] = NULL;//identityBone;
      }
      boneNode = boneNode->mParent;
    }
  }
  return boneNodes;
}

void AssimpToSAModels::processSkeletalAnimations(const aiScene* scene, sa::Skeleton* skeleton)
{
  std::map<std::string, std::string> SAJoints;
  //Create animations for the skeleton pose.
  for(auto j : skeleton->Joints)
  {
    SAJoints[j.first] = j.second.Name;
  }
  std::map<int, sa::AnimationModel<std::string> > SAanimationsMap;
  for(std::map<std::string, std::string>::value_type& saJoint : SAJoints) {
    for(unsigned int i = 0; i < scene->mNumAnimations; ++i)
    {
      SAanimationsMap[i].Name = scene->mAnimations[i]->mName.data;
      for(unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; ++j)
      {
        if(saJoint.second == scene->mAnimations[i]->mChannels[j]->mNodeName.data)
        {

          for(unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; ++k)
          {
            glm::vec3 T(	scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x,
                          scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y,
                          scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z);
            double time = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
            SAanimationsMap[i].Channels[saJoint.first].Translation.push_back(sa::AnimationChannelModel::TranslationKey((time/scene->mAnimations[i]->mTicksPerSecond), T));
          }
          for(unsigned int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; ++k)
          {
            glm::quat Q = glm::quat(
                  scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w,
                  scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x,
                  scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y,
                  scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z);
            double time = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
            SAanimationsMap[i].Channels[saJoint.first].Quaternion.push_back(sa::AnimationChannelModel::QuaternionKey((time/scene->mAnimations[i]->mTicksPerSecond), Q));
          }
        }
      }
    }
  }
  unsigned int i = 0;
  for(auto a : SAanimationsMap)
  {
    a.second.Duration = (scene->mAnimations[i]->mDuration / scene->mAnimations[i]->mTicksPerSecond);

    //Ticks per second shall be bigger than 0. If not we force 60.
    if(scene->mAnimations[i]->mTicksPerSecond <= 0.0001)
      a.second.Duration = (scene->mAnimations[i]->mDuration / 60.0);

    a.second.Name = scene->mAnimations[i]->mName.data;

    skeleton->Animations[scene->mAnimations[i]->mName.data] = (a.second);
    ++i;
  }
}
#include <scene_models/visitorapplytransformations.h>
#include <scene_models/visitorapplyanimation.h>
#include <scene_models/visitorgetmeshnodes.h>

sa::AABBModel AssimpToSAModels::calculateBoundingBox(bool haveBones, std::deque<sa::SubMeshModel*> saMeshes, std::deque<sa::AnimationModel<boost::uuids::uuid>* > animations, sa::NodeModel* saRoot) {
  sa::AABBModel totalBBox = calculateBoundingBoxAtSGTransformStep(saMeshes, saRoot);

  //Expand the box with node animations if they exist.
  float inc = 1.0f/60.0f;
  //Run the animations and calculate on bbox per each.
  for(const sa::AnimationModel<boost::uuids::uuid>* animation : animations) {
    //Run the animation in 60 Hz.
    float currentTime = 0.0f;
    while(currentTime < animation->Duration) {
      sa::AABBModel bbox = calculateBoundingBoxAtSGAnimationStep(saMeshes, animation, saRoot, currentTime);
      totalBBox.expand(bbox);
      currentTime += inc;
    }
  }

  //Expand the box with skeletal animations if they exist.
  if(haveBones) {
    sa::AABBModel bbox = calculateBoundingBoxAtSKAnimationStep(saMeshes, saRoot);
    //totalBBox.expand(bbox);
    totalBBox=bbox;
  }
  return totalBBox;
}

sa::AABBModel AssimpToSAModels::calculateBoundingBoxAtSGTransformStep(std::deque<sa::SubMeshModel*> saMeshes, sa::NodeModel* saRoot) {

  sa::VisitorApplyTransformations transformVisitor;
  saRoot->accept(&transformVisitor);

  sa::VisitorGetMeshNodes getVisitor;
  saRoot->accept(&getVisitor);

  sa::AABBModel totalBBox;
  bool firstTime = true;
  for(sa::MeshNodeModel* node : getVisitor.m_meshes) {
    sa::SubMeshModel* subMesh = saMeshes[node->mesh()];
    sa::AABBModel subMeshBBox = subMesh->getBoundingBox();
    subMeshBBox.transform(node->transformation());
    if(firstTime) {
      totalBBox = subMeshBBox;
      firstTime = false;
    }
    totalBBox.expand(subMeshBBox);
  }
  return totalBBox;
}

sa::AABBModel AssimpToSAModels::calculateBoundingBoxAtSGAnimationStep(std::deque<sa::SubMeshModel*> saMeshes, const sa::AnimationModel<boost::uuids::uuid>* animation, sa::NodeModel* saRoot, float time) {
//  std::map<boost::uuids::uuid, sa::SubMeshModel*> saMeshMap;
//  for(sa::SubMeshModel* saMesh : saMeshes) {
//    saMeshMap[saMesh->getKey()] = saMesh;
//  }

  sa::VisitorApplyAnimation animVisitor(time, animation);
  saRoot->accept(&animVisitor);

  sa::VisitorApplyTransformations transformVisitor;
  saRoot->accept(&transformVisitor);

  sa::VisitorGetMeshNodes getVisitor;
  saRoot->accept(&getVisitor);

  sa::AABBModel totalBBox = saMeshes[0]->getBoundingBox();
  bool firstTime = true;
  for(sa::MeshNodeModel* node : getVisitor.m_meshes) {
    sa::SubMeshModel* subMesh = saMeshes[node->mesh()];
    sa::AABBModel subMeshBBox = subMesh->getBoundingBox();
    subMeshBBox.transform(node->transformation());

    if(firstTime) {
      totalBBox = subMeshBBox;
      firstTime = false;
    }
    totalBBox.expand(subMeshBBox);
  }
  return totalBBox;
}

sa::AABBModel AssimpToSAModels::calculateBoundingBoxAtSKAnimationStep(std::deque<sa::SubMeshModel*> saMeshes, sa::NodeModel* saRoot) {
//  std::map<boost::uuids::uuid, sa::SubMeshModel*> saMeshMap;
//  for(sa::SubMeshModel* saMesh : saMeshes) {
//    saMeshMap[saMesh->getKey()] = saMesh;
//  }

  sa::VisitorApplyTransformations transformVisitor;
  saRoot->accept(&transformVisitor);

  sa::VisitorGetMeshNodes getVisitor;
  saRoot->accept(&getVisitor);


  sa::AABBModel totalAABB;

  bool firstTime = true;
  for(sa::MeshNodeModel* meshNode : getVisitor.m_meshes) {
    sa::SubMeshModel* subMesh = saMeshes[meshNode->mesh()];
    sa::Skeleton* skeleton = subMesh->skeleton();
    for(unsigned int animation = 0; animation < skeleton->getAnimationNames().size(); ++animation) {
      float time = 0.0f;
      while(time < skeleton->Animations[skeleton->getAnimationNames()[animation]].Duration)
      {
        skeleton->animate(time, skeleton->getAnimationNames()[animation]);
        skeleton->applyTransformations();
        for(sa::Skeleton::JointMap::value_type j : skeleton->Joints) {
          sa::AABBModel jointAABB;

          sa::JointModel joint = j.second;
          bool firstpoint = true;
          for(sa::JointModel::VertexWeight weight : joint.Weights) {
            if(firstpoint) {
              jointAABB = sa::AABBModel(subMesh->getVertices()[weight.VertexIndex].position(), glm::vec3(0,0,0));
              firstpoint = false;
            }
            sa::SubMeshModel::Vertex vertex = subMesh->getVertices()[weight.VertexIndex];
            jointAABB.expand(vertex.position());
          }
          if(firstpoint == false) {
            jointAABB.transform(meshNode->transformation() * joint.Transformation);
            if(firstTime) {
              totalAABB = jointAABB;
              firstTime = false;
            }
            totalAABB.expand(jointAABB);
          }
        }
        time+=1.0f/60.0f;
      }
    }
  }
  return totalAABB;
}
