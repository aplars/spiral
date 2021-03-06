#include "scene.h"
#include <omp.h>
#include <renderer_engine/rendercontext.h>
#include <math/intersectiontests.h>
#include <renderer_engine/image.h>
#include <renderer_engine/renderdevice.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "math/mat4ext.h"

namespace sa {

Scene::~Scene()
{
}

Scene::Scene(unsigned int width, unsigned int height, Config config)
  : m_screenWidth(width)
  , m_screenHeight(height)
  , m_config(config)
  , m_sun({1,0,1}, {0.75, 0.75, 0.75, 1}, {0.2, 0.2, 0.2, 1})
  , m_sky(5400, 5, 10, 1)
  , m_lightShafts(config)
  //, onePlant(10.5f, {glm::vec3(-100, 0, 0), glm::vec3(100, 0, 0)})
  , grass({-100.0f, -100.0f}, {100.0f, 100.0f}, 10.0f, 20.0f, 0.0f)
  , m_imageCache(1000)
  , m_textureCache(1000)
  , m_shaderCache(1000)

{
  m_weather.setWindDirection(glm::vec3(1.0f, 0.0f, 0.0f));
  m_weather.setWindStrenght(5.0f);
  setAtmosExposure(0.0034f);
  setAtmosDecay(1.0f);
  setAtmosDensity(0.84f);
  setAtmosWeight(5.65f);
  std::vector<float> shadowMapCascadeDistance;
  shadowMapCascadeDistance.push_back(50);
  shadowMapCascadeDistance.push_back(75);
  shadowMapCascadeDistance.push_back(250);
  shadowMapCascadeDistance.push_back(500);
  shadowMapCascadeDistance.push_back(3000);

  float aspect = width/static_cast<float>(height);
  if(height > width)
    aspect = height/static_cast<float>(width);

  m_shadowMapping.create(shadowMapCascadeDistance, aspect, 1024, 1024);


  m_projection = glm::perspective(sa::DegToRad(60.0f), aspect, 0.1f, m_shadowMapping.getShadowMapCascadeDistance().back());

}

void Scene::getPickRay(int winX, int winY, glm::vec3 &outOrigin, glm::vec3 &outDir) const
{
  glm::vec3 pickA = glm::unProject(
        glm::vec3(static_cast<float>(winX), static_cast<float>(m_screenHeight - winY), 0.0),
        m_camera.viewMatrix(),
        m_projection,
        glm::vec4(0, 0, m_screenWidth, m_screenHeight));

  glm::vec3 pickB = glm::unProject(
        glm::vec3(static_cast<float>(winX), static_cast<float>(m_screenHeight - winY), 1.0),
        m_camera.viewMatrix(),
        m_projection,
        glm::vec4(0, 0, m_screenWidth, m_screenHeight));

  outOrigin = pickA;
  outDir = glm::normalize(pickB - pickA);

}

glm::vec3 Scene::getScreenPosVsGroundPointoint(int winX, int winY) const
{
//  glm::vec3 origin;
//  glm::vec3 dir;
//  getPickRay(winX, winY, origin, dir);
  
//  foreach (StreamedMeshEntity* groundMesh, m_groundMeshes) {

//  }

}

void Scene::resize(unsigned int w, unsigned int h)
{
  m_screenWidth = w;
  m_screenHeight = h;
}

void Scene::setTime(double julianDay, double timeOfDay)
{
  m_sky.JulianDay = julianDay;
  m_sky.TimeOfDay = timeOfDay;
}

void Scene::setSunSimulationTimeScale(double timeScale) {
  m_sky.TimeScale = timeScale;
}

void Scene::runSunSimulation(bool runSimulation) {
  m_sky.IsRunningSimulation = runSimulation;
}

void Scene::setAtmosphereFogDensity(float desity)
{
  m_sky.FogDensity = desity;
}

void Scene::setAtmosExposure(float exposure)
{
  m_lightShafts.setExposure(exposure);
}

void Scene::setAtmosDecay(float decay)
{
  m_lightShafts.setDecay(decay);
}

void Scene::setAtmosDensity(float density)
{
  m_lightShafts.setDensity(density);
}

void Scene::setAtmosWeight(float weight)
{
  m_lightShafts.setWeight(weight);
}

void Scene::setSun(const DirectionalLight& sun) {
  m_sun = sun;
}

void Scene::setSunPosition(float phi, float theta) {
  float x = cos(phi) * sin(theta);
  float y = sin(phi) * sin(theta);
  float z = cos(theta);
  m_sun.setDirection(glm::vec3() - glm::vec3(x, y, z));
}

void Scene::addModel(const std::string& name, MeshRenderablePtr mesh, bool castShadow) {
  ModelPtr model = std::make_shared<Model>(mesh, castShadow);
//  AABBModel aabbmodel = streamedEntity->getBoundingBox();
//  addDebugBox(name+"db", aabbmodel.getCenter()[0], aabbmodel.getCenter()[1], aabbmodel.getCenter()[2],
//      aabbmodel.getHalfSize()[0], aabbmodel.getHalfSize()[1], aabbmodel.getHalfSize()[2]);
  models[name] = model;

 // streamedEntity->addPropertyChangedListener([&name, this](const StreamedMeshEntity::PropertyChangedEvent& /*evt*/) {
    //DebugEntityBox* box = getDebugBoxEntety(name+"db");
    //box->setPosition(evt.m_object->getBoundingBox().getCenter());
 // });
}

void Scene::addGroundModel(const std::string& name, MeshRenderablePtr mesh, bool castShadow) {
  ModelPtr model = std::make_shared<Model>(mesh, castShadow);
  models[name] = model;
  m_groundModels[name] = model;
}

void Scene::removeModel(const std::string& name) {
  //Add the mesh to the remove list.
  //The mesh will be deleted safely during update.
  //We need to do this to make sure to avoid thread collisions.
  m_modelsToDelete.push_back(name);
}

ModelPtr Scene::getMeshEntity(const std::string &name)
{
  return models[name];
}

void Scene::addDebugBox(const std::string& name, float posx, float posy, float posz, float hw, float hh, float hd) {
  m_debugBoxes[name] = new DebugEntityBox(posx, posy, posz, hw, hh, hd);
}

void Scene::toCPU() {
  std::array<PlaneT<float>, 6> frustum = m_camera.getFrustum(m_projection);

  for(Models::value_type e : models) {
    AABBModel bbox = e.second->getBoundingBox();
    IntersectionTests::Side side = IntersectionTests::FrustumAABBIntersect(frustum, bbox.getMin(), bbox.getMax());
    bool isInFrustums = (side == IntersectionTests::Inside || side == IntersectionTests::Intersect) | m_shadowMapping.isAABBVisibleFromSun(m_sunCamera, bbox.getMin(), bbox.getMax());

    if(isInFrustums) {
      if(e.second->currentDataStorage() == DataStorage::Disk)
      {
        //We don not want to load this sucker again therefore we set it as pending.
        //if not set to pending it will try to laod again when its alreadiy loading.
        e.second->setPendingStorage();
        background_worker::work_ptr work;
        work.reset(new background_worker::work());
        work->doWork = [=]() {
          e.second->toCPU(m_imageCache, m_config.getParam("DATA_DIR") + "/textures/", m_config.getParam("DATA_DIR") + "/shaders/");
        };
        work->workDone = []() {
        };

        m_meshLoader.push(*work);
      }
    }
    else {
      if(e.second->currentDataStorage() == DataStorage::GPU) {
        e.second->unload();
      }
    }
  }
  m_sunCamera.setLookAt(m_sun.direction(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

void Scene::toGPUOnce(RenderDevice* device, RenderContext* context) {
  if(m_firstTimeInToGPU) {
    if(m_shadowBufferTarget.size() <= 0) {
      for(unsigned int i = 0; i < m_shadowMapping.getNumberOfPasses(); i++) {
        m_shadowBufferTarget.push_back(context->createRenderDepthToTexture(m_shadowMapping.getShadowMapWidth(), m_shadowMapping.getShadowMapHeight()));
      }
    }

    m_sky.toGPU(m_config, device, context);
    m_lightShafts.toGPU(m_config, device, context);

    m_sunLightShaftsTarget = context->createRenderToTexture(m_screenWidth, m_screenHeight);

    grass.toGPU(m_config, m_shadowMapping.getNumberOfPasses(), device, context);

    //addDebugBox("thesun", m_sky.getSunPosition()[0], m_sky.getSunPosition()[1], m_sky.getSunPosition()[2], 100, 100, 100);
    //addDebugBox("thesun", 100, 0, 0, 10, 10, 10);
    m_firstTimeInToGPU = false;
  }
}

void Scene::toGPU(RenderDevice* device, RenderContext* context) {
    toGPUOnce(device, context);

  if(m_lightFrustumAabb) {
    if(!getDebugBoxEntety("pervoFrust"))
    {
      addDebugBox("pervoFrust", m_lightFrustumAabb->getCenter()[0], m_lightFrustumAabb->getCenter()[1], m_lightFrustumAabb->getCenter()[2], m_lightFrustumAabb->getHalfSize()[0], m_lightFrustumAabb->getHalfSize()[1], m_lightFrustumAabb->getHalfSize()[2]);
    }
  }

  for(Models::value_type e : models) {
    if(e.second->currentDataStorage() == DataStorage::CPU)
    {
      e.second->toGPU(m_config, m_shadowMapping.getNumberOfPasses(), m_textureCache, m_shaderCache, device, context);
    }
  }

  for(DebugBoxEntities::value_type e : m_debugBoxes) {
    e.second->toGPU(m_config, device, context);
  }
}

void Scene::deleteMeshes(Models meshes, Models groundMeshes)
{
  for(std::deque<std::string>::iterator toDeleteIt = m_modelsToDelete.begin(); toDeleteIt != m_modelsToDelete.end(); ) {
    Models::const_iterator findIt = meshes.find(*toDeleteIt);
    Models::const_iterator gmFindIt = groundMeshes.find(*toDeleteIt);
    if(findIt != meshes.end())
    {
      ModelPtr entity = findIt->second;
      if(entity->currentDataStorage() != DataStorage::Pending)
      {
        //delete entity;
        entity = nullptr;
        toDeleteIt = m_modelsToDelete.erase(toDeleteIt);
        meshes.erase(findIt);
        if(gmFindIt != groundMeshes.end())
          groundMeshes.erase(gmFindIt);
      }
      else {
        ++toDeleteIt;
      }
    }
    else {
      //The mesh does not exist. Remove it from the delete list.
      toDeleteIt = m_modelsToDelete.erase(toDeleteIt);
    }
  }
}

void Scene::update(float dt) {
  //Delete meshes that are not pending (loading)
  deleteMeshes(models, m_groundModels);


  std::deque<ModelPtr> entetiesDeq;
  for(Models::value_type e : models) {
    entetiesDeq.push_back(e.second);
  }


#pragma omp parallel for
  for(unsigned int i = 0; i < entetiesDeq.size(); ++i) {
    ModelPtr e = entetiesDeq[i];
    e->applyAnimations(dt);
  }

  m_sky.update(dt, glm::vec3(m_camera.eye()));

  m_sun.setDirection(glm::vec3(glm::normalize(m_sky.getSunPosition())));

  grass.update(dt);
  //onePlant.update(dt);
  //m_debugBoxes["thesun"]->setPosition(m_sky.getSunPosition());

  //m_camera.setLookAt(m_sky.getSunPosition(), glm::vec3(0,0,0), glm::vec3(0,1,0));
  //m_camera.setLookAt(m_camera.eye(), m_sky.getSunPosition(), glm::vec3(0,1,0));
  for(DebugBoxEntities::value_type db : m_debugBoxes)
  {
    db.second->update(dt);
  }
  m_currentTime+=dt;
}


void Scene::drawShadowPass(RenderContext* context) {
  m_shadowMapping.updateShadowPass(m_camera, m_sunCamera);


  DrawDataList allToDraw;

  for(Models::value_type e : models) {
    DrawDataList dds = e.second->getDrawData(RenderPass::Shadow);
    allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
  }


  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_sunViewMatrix"] = m_sunCamera.viewMatrix();
  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_directionalLight.direction"] = m_sun.direction();
  m_sceneSpecificShaderUniforms.Vec4Uniforms["u_directionalLight.diffuse"] = m_sun.diffuse();
  m_sceneSpecificShaderUniforms.Vec4Uniforms["u_directionalLight.ambient"] = m_sun.ambient();
  m_sceneSpecificShaderUniforms.FloatUniforms["u_fogDensity"] = m_sky.FogDensity;

  for(unsigned int shadowPass = 0; shadowPass < m_shadowMapping.getNumberOfPasses(); shadowPass++)
  {
    m_shadowBufferTarget[shadowPass]->bind();
    context->setViewport(m_shadowBufferTarget[shadowPass]->getWidth(), m_shadowBufferTarget[shadowPass]->getHeight());
    context->setCullFace(RenderContext::CullFace::Front);
    context->clear();

    m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_shadowProjectionMatrix"] = m_shadowMapping.getShadowMapProjections()[shadowPass];

    context->draw(allToDraw, m_sceneSpecificShaderUniforms);
    m_shadowBufferTarget[shadowPass]->release();
  }
}



void Scene::drawUberPass(RenderContext* context)
{
  context->setCullFace(RenderContext::CullFace::Back);
  context->setViewport(m_screenWidth, m_screenHeight);

  DrawDataList allToDraw;
  {
    DrawDataList dds = grass.getDrawData(RenderPass::Uber);
    allToDraw.insert(allToDraw.begin(), dds.begin(), dds.end());

  }
  allToDraw.push_back(m_sky.getDrawData(RenderPass::Uber));

  for(Models::value_type e : models) {
    DrawDataList dds = e.second->getDrawData(RenderPass::Uber);
    allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
  }

  for(const DebugBoxEntities::value_type e : m_debugBoxes) {

    allToDraw.push_back(e.second->getDrawData());
  }


  std::vector<unsigned int> shadowMap;
  for(unsigned int i = 0; i < m_shadowMapping.getNumberOfPasses(); ++i) {
    shadowMap.push_back(4+i);
  }

  for(DrawDataList::value_type& dd : allToDraw) {

    for(unsigned int i = 0; i < m_shadowMapping.getNumberOfPasses(); ++i) {
      dd.TEX[4+i] = m_shadowBufferTarget[i]->getDepthTexture();
    }
  }

  m_sceneSpecificShaderUniforms.Sampler2DArrayUniforms["u_shadowMap"] = shadowMap;
  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_viewMatrix"] = m_camera.viewMatrix();
  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_projectionMatrix"] = m_projection;
  m_sceneSpecificShaderUniforms.Matrix4ArrayUniforms["u_depthBiasMVPMatrix"] = m_shadowMapping.getDepthBiasMVPMatrix();
  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_directionalLight.direction"] = m_sun.direction();
  m_sceneSpecificShaderUniforms.Vec4Uniforms["u_directionalLight.diffuse"] = m_sun.diffuse();
  m_sceneSpecificShaderUniforms.Vec4Uniforms["u_ambientColor"] = m_ambientColor;

  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_eyePosition"] = m_camera.eye();
  m_sceneSpecificShaderUniforms.FloatArrayUniforms["u_shadowMapCascadeDistance"] = m_shadowMapping.getShadowMapCascadeDistance();
  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_sunPosition"] = m_sky.getSunPosition();
  m_sceneSpecificShaderUniforms.FloatUniforms["u_windStrenght"] = m_weather.windStrenght();
  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_windDirection"] = m_weather.windDirection();
  m_sceneSpecificShaderUniforms.FloatUniforms["u_time"] = m_currentTime;
  context->draw(allToDraw, m_sceneSpecificShaderUniforms);
}

void Scene::createLightShaftsPass(RenderContext *context)
{
  m_sunLightShaftsTarget->bind();
  context->setCullFace(RenderContext::CullFace::Back);
  context->setViewport(m_sunLightShaftsTarget->getWidth(), m_sunLightShaftsTarget->getHeight());
  context->clear();
  DrawDataList allToDraw;

  for(Models::value_type e : models) {
    DrawDataList dds = e.second->getDrawData(RenderPass::SunLightShafts);
    allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
  }

  //Add the sun drawable
  DrawData skyDd = m_sky.getDrawData(RenderPass::SunLightShafts);
  allToDraw.push_back(skyDd);

  {
    DrawDataList dds = grass.getDrawData(RenderPass::SunLightShafts);
    allToDraw.insert(allToDraw.begin(), dds.begin(), dds.end());
  }


  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_viewMatrix"] = m_camera.viewMatrix();
  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_projectionMatrix"] = m_projection;
  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_sunPosition"] = m_sky.getSunPosition();

  context->draw(allToDraw, m_sceneSpecificShaderUniforms);
  m_sunLightShaftsTarget->release();

}

void Scene::drawLightShaftsPass(RenderContext *context)
{
  glm::vec3 sunPositionInScreenCoords = glm::project(
        m_sky.getSunPosition() + glm::vec3(m_camera.eye().x, 0.0f, m_camera.eye().z),
        m_camera.viewMatrix(),
        m_projection,
        glm::vec4(0, 0, m_sunLightShaftsTarget->getWidth(), m_sunLightShaftsTarget->getHeight()));

  sunPositionInScreenCoords.x /= m_sunLightShaftsTarget->getWidth();
  sunPositionInScreenCoords.y /= m_sunLightShaftsTarget->getHeight();
  if(sunPositionInScreenCoords.x < -0.1f || sunPositionInScreenCoords.x > 1.1f)
    return;
  if(sunPositionInScreenCoords.y < -0.1f || sunPositionInScreenCoords.y > 1.1f)
    return;

  context->setCullFace(RenderContext::CullFace::Back);
  context->setViewport(m_screenWidth, m_screenHeight);
  DrawDataList dds;
  DrawData lightShaftsDD = m_lightShafts.getDrawData();
  lightShaftsDD.TEX[0] = m_sunLightShaftsTarget->getTexture();

  dds.push_back(lightShaftsDD);

  m_sceneSpecificShaderUniforms.Sampler2DUniforms["u_texture"] = 0;
  m_sceneSpecificShaderUniforms.Vec2Uniforms["u_sunPositionOnScreen"] = glm::vec2(sunPositionInScreenCoords.x, sunPositionInScreenCoords.y);
  context->draw(dds, m_sceneSpecificShaderUniforms);

}

void Scene::draw(RenderContext* context) {
  drawShadowPass(context);
  createLightShaftsPass(context);

  drawUberPass(context);
  drawLightShaftsPass(context);
}
}
