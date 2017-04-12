#ifndef ASSIMPTOSAMODELS_H
#define ASSIMPTOSAMODELS_H
#include <scene_models/submeshmodel.h>
#include <scene_models/materialmodel.h>
#include <scene_models/transformationnodemodel.h>
#include <scene_models/animationmodel.h>
#include <scene_models/aabbmodel.h>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/ai_assert.h>
#include <assimp/cfileio.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/IOSystem.hpp>
#include <assimp/IOStream.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/DefaultLogger.hpp>

#include <boost/uuid/uuid.hpp>
#include <QDir>
#include <deque>

namespace sa {
class Skeleton;
typedef std::shared_ptr<Skeleton> SkeletonPtr;
}

class AssimpToSAModels
{
public:
  static bool convertToXML(bool toXML, const QDir& sourceDir, const QDir& destDir, bool isStatic, float scaleFactor);

  static std::deque<sa::MaterialModel*> processMaterials(const aiScene* const scene);

  static std::deque<sa::SubMeshModel*> processMeshes(const aiScene* const scene);

  static sa::NodeModel* processTransformationTree(const aiNode* const currentNode,
                                                  std::map<std::string, boost::uuids::uuid>& nodeKeys, unsigned int &numMeshes);

  static  std::deque<sa::AnimationModel<boost::uuids::uuid> * > processAnimations(const aiScene* const scene,
                                                            const std::map<std::string,
                                                            boost::uuids::uuid>& nodeKeys);

  static bool processBones(const aiScene* const scene,
                           std::deque<sa::SubMeshModel*> saMeshes,
                           const std::map<std::string, boost::uuids::uuid>& nodeKeys);

  static std::deque<sa::Skeleton*> processSkeletonsForMeshNode(const aiScene* scene, aiNode* const meshNode);

  static void processSkeletalAnimations(const aiScene* scene, sa::Skeleton* skeleton);

  static sa::AABBModel calculateBoundingBox(bool haveBones, std::deque<sa::SubMeshModel*> saMeshes, std::deque<sa::AnimationModel<boost::uuids::uuid>* > animations, sa::NodeModel* saRoot);

  static sa::AABBModel calculateBoundingBoxAtSGTransformStep(std::deque<sa::SubMeshModel*> saMeshes, sa::NodeModel* saRoot);

  static sa::AABBModel calculateBoundingBoxAtSGAnimationStep(std::deque<sa::SubMeshModel*> saMeshes, const sa::AnimationModel<boost::uuids::uuid>* animation, sa::NodeModel* saRoot, float time);

  static sa::AABBModel calculateBoundingBoxAtSKAnimationStep(std::deque<sa::SubMeshModel*> saMeshes, sa::NodeModel* saRoot);

private:
  template <typename T>
  static void GetMaterialProperty(const aiMaterial* material, const char* pKey,unsigned int type, unsigned int idx, T defaultV, T& pOut)
  {
    if(material->Get(pKey, type, idx, pOut) != AI_SUCCESS)
      pOut = defaultV;
  }

  static sa::MaterialModel::BlendMode getSaBlendModeFromAi(aiBlendMode aiBlendMode) {
    switch(aiBlendMode) {
    case aiBlendMode_Default:
      return sa::MaterialModel::BlendMode::Default;
      break;
    case aiBlendMode_Additive:
      return sa::MaterialModel::BlendMode::Additive;
      break;
    case _aiBlendMode_Force32Bit:
      return sa::MaterialModel::BlendMode::Force32Bit;
      break;

    }
  }

  static glm::vec3 getGlmVector3FromAi(const aiVector3D& v) {
    glm::vec3 ai(v.x, v.y, v.z);
    return ai;
  }

  static sa::QuaternionT<float> getSaQuatFromAi(const aiQuaternion& q) {
    sa::QuaternionT<float> ai(q.x, q.y, q.z, q.w);
    return ai;
  }

  static glm::mat4 getSaMatrixFromAi(const aiMatrix4x4& aim) {
    glm::mat4 sam;
    for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 4; ++j)
        sam[i][j] = aim[j][i];
    return sam;
  }

  static glm::vec4 getSaColorFromAi(const aiColor4D& aic) {
    glm::vec4 sac;
    sac[0] = aic.r;
    sac[1] = aic.g;
    sac[2] = aic.b;
    sac[3] = aic.a;
    return sac;
  }

  static sa::MaterialModel::TextureMappingMode getSaTexMapModeFromAi(aiTextureMapMode mapMode) {
    switch(mapMode) {
    case aiTextureMapMode_Wrap:
      return sa::MaterialModel::TextureMappingMode::Repeat;
    case aiTextureMapMode_Clamp:
        return sa::MaterialModel::TextureMappingMode::ClampToEdge;
    case aiTextureMapMode_Decal:
      return sa::MaterialModel::TextureMappingMode::ClampToBorder;
      break;
    case aiTextureMapMode_Mirror:
      return sa::MaterialModel::TextureMappingMode::MirroredRepeat;
      break;
    default:
      return sa::MaterialModel::TextureMappingMode::Repeat;

    }
  }

  static std::map<aiNode* const, const aiBone*> getBoneNodes(const aiScene* scene, aiNode* const meshNode, aiMesh* const mesh);
};

#endif // ASSIMPTOSAMODELS_H
