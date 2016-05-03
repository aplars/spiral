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
  StreamedMeshEntity(const std::string& resourcePath, const std::string& resourceName);
  void setPosition(const Vector3T<float>& position);
  void setPosition(float x, float y, float z);
  const Vector3T<float>& getPosition() const;

  void setHeading(float headingInRad);
  float getHeading() const;

  AABBModel getBoundingBox() const;
  DataStorage currentDataStorage() const;
  void setPendingStorage();
  void setDiskStorage();
  void toCPU(ImageCache& imageCache);
  void toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context);
  void applyAnimations(float dt);
  void applyTransformations();
  //Returns a list of all skeletal animations
  std::deque<std::string> getSkeletalAnimations() const;
  //play's an skeletal animation.
  void playSkeletalAnimation(const std::string& animationName);

  //Returns a list of all node animations
  std::deque<std::string> getNodeAnimations() const;
  //play's an node animation.
  void playNodeAnimation(const std::string& animationName);

  void unloadGPU();
  void unloadCPU();
  /**
   * @brief getDrawData returns the data needed to draw the mesh.
   * @return the data needed to render the mesh.
   */
  std::deque<DrawData>& getDrawData();

private:
  MeshRenderablePtr m_mesh;
  Vector3T<float> m_position;
  float m_heading = 0.0f;
  DataStorage m_currentStorage;
  mutable std::mutex m_mutex;
};
}
