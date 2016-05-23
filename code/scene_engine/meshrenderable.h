#pragma once
#include "datastorage.h"
#include <scene_models/meshmodel.h>
#include <renderer_engine/drawdata.h>
#include <renderer_engine/image.h>
#include <string>
#include "adt/lru.h"
#include "imagecache.h"
#include "texturecache.h"
#include "shadercache.h"

namespace sa {
class RenderDevice;
class RenderContext;
class VertexBuffer;
class ConfigurationManager;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
class MeshRenderable {
public:
  MeshRenderable(const std::string& resourcePath, const std::string& resourceName);

  const AABBModel& getBoundingBox() const;

  /**
   * @brief currentDataStaorage is telling us where the mesh are stored at the time as this method is called.
   * @return where the mesh is stored. Ex. On disk or on primary memory.
   */
  //DataStorage currentDataStorage() const { return m_currentDataStorage; }

  /**
   * @brief toCPU loads the resource from HD to model (main memory).
   */
  void toCPU(ImageCache& imageCache);

  /**
   * @brief toGPU loads the model into the graphics memory. The model is not released when put on the graphics card.
   * @param device
   * @param context
   * @return
   */
  void toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context);

  void unloadGPU();
  void unloadCPU();
  /**
   * @brief update updates the animations and applies them.
   * @param currentTime the current global time.
   */
  //void update(float currentTime);

  /**
   * @brief getDrawData returns the data needed to draw the mesh.
   * @return the data needed to render the mesh.
   */
  const std::deque<sa::DrawData>& getDrawData() const { return m_drawDataDeque; }

  std::deque<sa::DrawData>& getDrawData() { return m_drawDataDeque; }

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
private:
  //DataStorage m_currentDataStorage = DataStorage::Disk;
  std::string m_resourcePath;
  std::string m_resourceName;
  std::map<unsigned int, DrawData>  m_drawData;
  std::deque<DrawData> m_drawDataDeque;
  MeshModel m_meshModel;
  std::map<unsigned int, Image> m_ambientImage;
  std::map<unsigned int, Image> m_diffuseImage;
  std::map<unsigned int, Image> m_specularImage;

  //int m_modelMatrixUniform = 0;

  float m_currentSkeletalAnimationTime = 0.0f;
  std::string m_currentSkeletalAnimation = "__NO_FUCKING_ANIMATION_AT_ALL__";

  float m_currentNodeAnimationTime = 0.0f;
  std::string m_currentNodeAnimation = "__NO_FUCKING_ANIMATION_AT_ALL__";
};

typedef std::shared_ptr<MeshRenderable> MeshRenderablePtr;
}
