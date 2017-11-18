#pragma once

#include "model.h"
#include "debugentity.h"
#include <deque>
#include <map>

#include "fpscamera.h"
#include "directionallight.h"
#include <adt/backgroundworker.h>
#include <adt/lru.h>
#include "imagecache.h"
#include "shadercache.h"
#include "shadows/shadowmapping.h"
#include "meshrenderable.h"
#include "sky/sky.h"
#include "sky/lightshafts.h"
#include "grass/grass.h"

#include "weather/weather.h"

#include <config/config.h>

namespace sa {
class RenderDevice;
class RenderContext;
class RenderDepthToTexture;
typedef std::shared_ptr<RenderDepthToTexture> RenderDepthToTexturePtr;
class RenderToTexture;
typedef std::shared_ptr<RenderToTexture> RenderToTexturePtr;

class Scene
{
public:
  ~Scene();
  Scene(unsigned int width, unsigned int height, Config config);

  void getPickRay(int winX, int winY, glm::vec3 &outOrigin, glm::vec3 &outDir) const;

  /**
   * @brief getWinPosVsGroundPointoint - returns the point where the (winX, winY)-screen position intersects the ground
   * @param winX - x-element of screen position
   * @param winY - y-element of screen position
   * @return returns the intersection point
   */
  glm::vec3 getScreenPosVsGroundPointoint(int winX, int winY) const;

  void resize(unsigned int w, unsigned int h);
  void setTime(double julianDay, double timeOfDay);
  void setSunSimulationTimeScale(double timeScale);
  void runSunSimulation(bool runSimulation);
  void setAtmosphereFogDensity(float desity);
  void setAtmosExposure(float exposure);
  void setAtmosDecay(float decay);
  void setAtmosDensity(float density);
  void setAtmosWeight(float weight);
  FPSCamera& camera() { return m_camera; }
  void addMeshEntity(const std::string& name, MeshRenderablePtr mesh, bool castShadow);
  void addGroundMeshEntity(const std::string& name, MeshRenderablePtr mesh, bool castShadow);
  void removeMeshEntity(const std::string& name);
  Model* getMeshEntity(const std::string& name)
  {
    return m_meshes[name];
  }
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
  void drawUberPass(RenderContext* context);
  void createLightShaftsPass(RenderContext* context);
  void drawLightShaftsPass(RenderContext* context);
  void draw(RenderContext* context);
  bool m_createLightFrustum = false;
  
private:
  void setSun(const DirectionalLight& sun);
  void setSunPosition(float phi, float theta);

  //void createShadowBufferRectangle(RenderDevice* device, RenderContext* context, float posx, float posy, float sw, float sh);


  typedef std::map<std::string, Model*> Models;
  typedef std::map<std::string, DebugEntityBox*> DebugBoxEntities;

  unsigned int m_screenWidth, m_screenHeight;

  Config m_config;

  bool m_firstTimeInToGPU = true;

  glm::mat4 m_projection;

  ShadowMapping m_shadowMapping;

  FPSCamera m_camera;
  FPSCamera m_sunCamera;
  DirectionalLight m_sun;
  glm::vec4 m_ambientColor = {0.5, 0.5, 0.5, 1.0};
  Models m_meshes;
  Models m_groundMeshes;
  std::deque<std::string> m_meshesToDelete;
  DebugBoxEntities m_debugBoxes;
  sky::Sky m_sky;
  Weather m_weather;
  LightShafts m_lightShafts;
  Grass grass;

  float m_currentTime = 0.0f;

  std::vector<RenderDepthToTexturePtr> m_shadowBufferTarget;
  RenderToTexturePtr m_sunLightShaftsTarget;

  AABBModel* m_lightFrustumAabb = nullptr;

  background_worker m_meshLoader;


  ImageCache m_imageCache;
  TextureCache m_textureCache;
  ShaderCache m_shaderCache;

  ShaderUniforms m_sceneSpecificShaderUniforms;
  void deleteMeshes(Models meshes, Models groundMeshes);
};

typedef std::shared_ptr<Scene> ScenePtr;
}
