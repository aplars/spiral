#pragma once
#include <memory>
#include <math/Vector3T.h>
#include <deque>
#include <renderer_engine/drawdata.h>
#include <scene_models/aabbmodel.h>
#include "datastorage.h"
#include <mutex>
#include "adt/lru.h"
#include <config/config.h>
#include "imagecache.h"
#include "texturecache.h"
#include "shadercache.h"
#include "meshrenderable.h"

namespace sa {
class RenderDevice;
class RenderContext;
class MeshRenderable;
typedef std::shared_ptr<MeshRenderable> MeshRenderablePtr;
class StreamedMeshEntity
{
public:
  ~StreamedMeshEntity();
  StreamedMeshEntity() {}
  StreamedMeshEntity(const std::string& resourcePath, const std::string& resourceName, bool castShadow);
  StreamedMeshEntity(MeshRenderablePtr mesh, bool castShadow);

  std::string getName() const { return m_mesh->getName(); }

  void setPosition(const Vector3T<float>& position);
  void setPosition(float x, float y, float z);
  const Vector3T<float>& getPosition() const;

  void setHeading(float headingInRad);
  float getHeading() const;

  AABBModel getBoundingBox() const;
  DataStorage currentDataStorage() const;
  void setPendingStorage();
  void setDiskStorage();
  void toCPU(ImageCache& imageCache, const std::string& dataDir, const std::__cxx11::string &shaderPath);
  void toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context);
  void applyAnimations(float dt);
  //Returns a list of all skeletal animations
  std::deque<std::string> getSkeletalAnimations() const;
  //play's an skeletal animation.
  void playSkeletalAnimation(const std::string& animationName);

  //Returns a list of all node animations
  std::deque<std::string> getNodeAnimations() const;
  //play's an node animation.
  void playNodeAnimation(const std::string& animationName);

  void unload();

  bool getCastShadow() const;
  /**
   * @brief getDrawData returns the data needed to draw the mesh.
   * @return the data needed to render the mesh.
   */
  DrawDataList getDrawData(RenderPass pass);


private:
  MeshRenderablePtr m_mesh;
  bool m_castShadow = false;
  DataStorage m_currentStorage;

  AABBModel m_boundingBox;
  float m_nodeAnimationTime = 0;
  float m_skeletonAnimationTime = 0;
  std::string m_nodeAnimation = "NO_FUCKING_ANIMATION";
  std::string m_skeletonAnimation = "NO_FUCKING_ANIMATION";

  std::deque<MeshTransform> m_transforms;

  DrawDataList m_drawData;

  Vector3T<float> m_position;
  float m_heading = 0.0f;
  mutable std::mutex m_mutex;
};
}
