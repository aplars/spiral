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
#include "renderpass.h"

namespace sa {
class RenderDevice;
class RenderContext;
class VertexBuffer;
class ConfigurationManager;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;

class MeshTransform {
public:
  MeshTransform(unsigned int mesh, const std::string& uniform, const Matrix44T<float>& transform)
    : Mesh(mesh)
    , Uniform(uniform)
    , Transform(transform)

  { }

  unsigned int Mesh = 0;
  std::string Uniform = "";
  Matrix44T<float> Transform;
};

class MeshRenderable {
public:

  MeshRenderable(const std::string& resourcePath, const std::string& resourceName);

  const std::string& getName() const { return m_resourceName; }
  const AABBModel& getBoundingBox() const;

  /**
   * @brief currentDataStaorage is telling us where the mesh are stored at the time as this method is called.
   * @return where the mesh is stored. Ex. On disk or on primary memory.
   */
  //DataStorage currentDataStorage() const { return m_currentDataStorage; }

  /**
   * @brief toCPU loads the resource from HD to model (main memory).
   */
  void toCPU(ImageCache& imageCache, const std::string& texturePath, const std::__cxx11::string &shaderPath);

  /**
   * @brief toGPU loads the model into the graphics memory. The model is not released when put on the graphics card.
   * @param device
   * @param context
   * @return
   */
  void toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context);

  void unload();
  /**
   * @brief update updates the animations and applies them.
   * @param currentTime the current global time.
   */
  //void update(float currentTime);

  /**
   * @brief getDrawData returns the data needed to draw the mesh.
   * @return the data needed to render the mesh.
   */
  sa::DrawDataList getDrawData(RenderPass pass);


  void setAnimationFrame(const std::string& skeletalAnimationName, const std::string& nodeAnimationName, float currentSkeletalAnimationTime, float currentNodeAnimationTime);
  void applyTransformations();

  //Returns a list of all skeletal animations
  std::deque<std::string> getSkeletalAnimations() const;

  //Returns a list of all node animations
  std::deque<std::string> getNodeAnimations() const;

private:
  unsigned int m_numberOfInstances = 0;
  DataStorage m_currentDataStorage = DataStorage::Disk;
  std::string m_resourcePath;
  std::string m_resourceName;
  std::map<unsigned int, DrawData>  m_drawData;
  DrawDataList m_drawDataDeque;
  MeshModel m_meshModel;
  std::map<unsigned int, Image> m_ambientImage;
  std::map<unsigned int, Image> m_diffuseImage;
  std::map<unsigned int, Image> m_specularImage;
  Image m_atmosphereFogImg;
  std::string m_vshCode;
  std::string m_fshCode;
  std::string m_spKey;

  std::string m_vshShadowCode;
  std::string m_fshShadowCode;
  std::string m_spShadowKey;

  std::string m_vshSunLightShaftsCode;
  std::string m_fshSunLightShaftsCode;
  std::string m_spSunLightShaftsKey;
};

typedef std::shared_ptr<MeshRenderable> MeshRenderablePtr;
}
