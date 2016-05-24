#ifndef SCENE_H
#define SCENE_H
#include "streamedmeshentity.h"
#include "debugentity.h"
#include <deque>
#include <map>

#include "fpscamera.h"
#include "directionallight.h"
#include "backgroundworker.h"
#include "adt/lru.h"
#include "imagecache.h"
#include "shadercache.h"
#include <config/config.h>
#include <boost/flyweight.hpp>

namespace sa {
class RenderDevice;
class RenderContext;
class RenderDepthToTexture;
typedef std::shared_ptr<RenderDepthToTexture> RenderDepthToTexturePtr;

class Scene
{
public:
  ~Scene();
  Scene(unsigned int width, unsigned int height);

  void initialize(RenderDevice* device);

  FPSCamera& camera() { return m_camera; }
  void setSun(const DirectionalLight& sun);
  void addMeshEntity(const std::string& name, const std::string& resourceName);
  void removeMeshEntity(const std::string& name);
  StreamedMeshEntity* getMeshEntity(const std::string& name) { return m_meshes[name]; }
  DebugEntityBox* getDebugBoxEntety(const std::string& name) {
    DebugBoxEntities::const_iterator it = m_debugBoxes.find(name);
    if(it == m_debugBoxes.end())
      return nullptr;
    return (*it).second;

  }

  void addDebugBox(const std::string& name, float posx, float posy, float posz, float hw, float hh, float hd);
  void toCPU();
  void toGPUOnce(RenderDevice* device, RenderContext* context);
  void toGPU(RenderDevice* device, RenderContext* context);
  void update(float dt);
  void drawShadowPass(RenderContext* context);
  void draw(RenderContext* context);
  bool m_createLightFrustum = false;
private:
  void createShadowBufferRectangle(RenderDevice* device, RenderContext* context, float posx, float posy, float sw, float sh);


  typedef std::map<std::string, StreamedMeshEntity*> Entities;
  typedef std::map<std::string, DebugEntityBox*> DebugBoxEntities;
  ConfigurationManager m_config;

  bool m_firstTimeInToGPU = true;

  std::vector<float> m_shadowMapCascadeDistance;

  std::vector<Matrix44T<float>> m_depthBiasMVPMatrix;
  Matrix44T<float> m_projection;
  std::vector<Matrix44T<float>> m_shadowProjections;

  FPSCamera m_camera;
  FPSCamera m_sunCamera;
  DirectionalLight m_sun;
  Entities m_meshes;
  std::deque<std::string> m_meshesToDelete;
  DebugBoxEntities m_debugBoxes;
  float m_currentTime = 0.0f;

  std::vector<RenderDepthToTexturePtr> m_shadowBufferTarget;
  std::shared_ptr<DrawData> m_shadowBufferRectangle;
  AABBModel* m_lightFrustumAabb = nullptr;

  BackgroundWorker m_meshLoader;


  ImageCache m_imageCache;
  TextureCache m_textureCache;
  ShaderCache m_shaderCache;

  ShaderUniforms m_sceneSpecificShaderUniforms;

};
}
#endif // SCENE_H
