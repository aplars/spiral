#include "scene.h"
#include <omp.h>
#include <renderer_engine/rendercontext.h>
#include <math/intersectiontests.h>
#include <renderer_engine/image.h>
#include <renderer_engine/renderdevice.h>


namespace sa {
Scene::~Scene()
{
  for(Entities::value_type e : m_meshes) {
    StreamedMeshEntity* entity = e.second;
    delete entity;
    entity = nullptr;
  }
}

Scene::Scene(unsigned int width, unsigned int height)
  : m_sun({1,0,1}, {0.75, 0.75, 0.75, 1}, {0.2, 0.2, 0.2, 1})
  , m_sky(5400, 5, 10, 1)
  , m_imageCache(1000)
  , m_textureCache(1000)
  , m_shaderCache(1000)
{
  std::vector<float> shadowMapCascadeDistance;
  shadowMapCascadeDistance.push_back(50);
  shadowMapCascadeDistance.push_back(75);
  shadowMapCascadeDistance.push_back(250);
  shadowMapCascadeDistance.push_back(500);
  shadowMapCascadeDistance.push_back(3000);

  //shadowMapCascadeDistance.push_back(200);

  float aspect = width/static_cast<float>(height);
  if(height > width)
    aspect = height/static_cast<float>(width);

  m_shadowMapping.create(shadowMapCascadeDistance, aspect, 1024, 1024);

  m_projection = sa::Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0f), aspect, 0.1, m_shadowMapping.getShadowMapCascadeDistance().back());

  m_config.init("sa_config.conf");
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

void Scene::setUseStableShadowMapping(bool stable)
{
  m_shadowMapping.setStable(stable);
}

void Scene::setSun(const DirectionalLight& sun) {
  m_sun = sun;
}

void Scene::setSunPosition(float phi, float theta) {
  float x = cos(phi) * sin(theta);
  float y = sin(phi) * sin(theta);
  float z = cos(theta);
  m_sun.setDirection(Vector3T<float>() - Vector3T<float>(x, y, z));
}

void Scene::addMeshEntity(const std::string& name, MeshRenderablePtr mesh, bool castShadow) {
  //MeshRenderablePtr mesh;
  //mesh.reset(new MeshRenderable(m_config.getParam("DATA_DIR") + "/meshes/", resourceName));
  m_meshes[name] = new StreamedMeshEntity(mesh, castShadow);
//  addDebugBox(name+"db", mesh->getBoundingBox().getCenter()[0], mesh->getBoundingBox().getCenter()[1], mesh->getBoundingBox().getCenter()[1],
//      mesh->getBoundingBox().getHalfSize()[0], mesh->getBoundingBox().getHalfSize()[1], mesh->getBoundingBox().getHalfSize()[2]);
}

void Scene::removeMeshEntity(const std::string& name) {
  //Add the mesh to the remove list.
  //The mesh will be deleted safely during update.
  //We need to do this to make sure to avoid thread collisions.
  m_meshesToDelete.push_back(name);
}

void Scene::addDebugBox(const std::string& name, float posx, float posy, float posz, float hw, float hh, float hd) {
  m_debugBoxes[name] = new DebugEntityBox(posx, posy, posz, hw, hh, hd);
}

void Scene::toCPU() {
  std::array<PlaneT<float>, 6> frustum = m_camera.getFrustum(m_projection);

  for(Entities::value_type e : m_meshes) {
    AABBModel bbox = e.second->getBoundingBox();
    IntersectionTests::Side side = IntersectionTests::FrustumAABBIntersect(frustum, bbox.getMin()-Vector3T<float>(2, 2, 2), bbox.getMax()+Vector3T<float>(2, 2, 2));
    bool isInFrustums = (side == IntersectionTests::Inside || side == IntersectionTests::Intersect);
    isInFrustums = isInFrustums | m_shadowMapping.isAABBVisibleFromSun(m_sunCamera, bbox.getMin()-Vector3T<float>(2, 2, 2), bbox.getMax()+Vector3T<float>(2, 2, 2));

    if(isInFrustums) {
      if(e.second->currentDataStorage() == DataStorage::Disk)
      {
        //qDebug() << "is in frustum: " << e.first.c_str();
        //We don not want to load this sucker again therefore we set it as pending.
        //if not set to pending it will try to laod again when its alreadiy loading.
        e.second->setPendingStorage();
        BackgroundWorkPtr work;
        work.reset(new BackgroundWork(e.second));
        work->doWork = [=](StreamedMeshEntity* entity) {
          entity->toCPU(m_imageCache, m_config.getParam("DATA_DIR") + "/textures/", m_config.getParam("DATA_DIR") + "/shaders/");
        };
        work->workDone = [](StreamedMeshEntity* /*entity*/) {
        };

        m_meshLoader.push(work);
      }
    }
    else {
      //qDebug() << "is NOT in frustum: " << e.first.c_str();
      if(e.second->currentDataStorage() == DataStorage::GPU) {
        e.second->unload();
      }
    }
  }

  Vector3T<float> centerpoint = m_camera.getFrusumCenterPoint(m_projection);

  //m_sunCamera.setLookAt(centerpoint + m_sun.direction(), centerpoint, Vector3T<float>(0, 1, 0));
  m_sunCamera.setLookAt(m_sun.direction(), Vector3T<float>(0, 0, 0), Vector3T<float>(0, 1, 0));
}

void Scene::toGPUOnce(RenderDevice* device, RenderContext* context) {
  if(m_firstTimeInToGPU) {
    if(m_shadowBufferTarget.size() <= 0) {
      for(int i = 0; i < m_shadowMapping.getNumberOfPasses(); i++) {
        m_shadowBufferTarget.push_back(context->createRenderDepthToTexture(m_shadowMapping.getShadowMapWidth(), m_shadowMapping.getShadowMapHeight()));
      }
    }

    m_sky.toGPU(m_config, device, context);
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

  for(Entities::value_type e : m_meshes) {
    if(e.second->currentDataStorage() == DataStorage::CPU)
    {
      e.second->toGPU(m_config, m_shadowMapping.getNumberOfPasses(), m_textureCache, m_shaderCache, device, context);
    }
  }

  for(DebugBoxEntities::value_type e : m_debugBoxes) {
    e.second->toGPU(m_config, device, context);
  }
}

void Scene::update(float dt) {
  //Delete meshes that are not pending (loading)
  for(std::deque<std::string>::iterator toDeleteIt = m_meshesToDelete.begin(); toDeleteIt != m_meshesToDelete.end(); ) {
    Entities::const_iterator findIt = m_meshes.find(*toDeleteIt);
    if(findIt != m_meshes.end())
    {
      StreamedMeshEntity* entity = findIt->second;
      if(entity->currentDataStorage() != DataStorage::Pending)
      {
        delete entity;
        entity = nullptr;
        toDeleteIt = m_meshesToDelete.erase(toDeleteIt);
        m_meshes.erase(findIt);
      }
      else {
        ++toDeleteIt;
      }
    }
    else {
      //The mesh does not exist. Remove it from the delete list.
      toDeleteIt = m_meshesToDelete.erase(toDeleteIt);
    }
  }

  std::deque<StreamedMeshEntity*> entetiesDeq;
  for(Entities::value_type e : m_meshes) {
    entetiesDeq.push_back(e.second);
  }


//#pragma omp parallel for
  for(unsigned int i = 0; i < entetiesDeq.size(); ++i) {
    StreamedMeshEntity* e = entetiesDeq[i];
    e->applyAnimations(dt);
  }
//#pragma omp parallel for
//  for(unsigned int i = 0; i < entetiesDeq.size(); ++i) {
//    StreamedMeshEntity* e = entetiesDeq[i];
//    if(e->currentDataStorage() == DataStorage::GPU)
//      e->applyTransformations();
//  }

  m_sky.update(dt, m_camera.eye());

  m_sun.setDirection(m_sky.getSunPosition().GetNormalized());

  //m_debugBoxes["thesun"]->setPosition(m_sky.getSunPosition());

  //m_camera.setLookAt(m_sky.getSunPosition(), Vector3T<float>(0,0,0), Vector3T<float>(0,1,0));
  //m_camera.setLookAt(m_camera.eye(), m_sky.getSunPosition(), Vector3T<float>(0,1,0));
  for(DebugBoxEntities::value_type db : m_debugBoxes)
  {
    db.second->update(dt);
  }
  m_currentTime+=dt;
}


void Scene::drawShadowPass(RenderContext* context) {
  DrawDataList allToDraw;
  for(Entities::value_type e : m_meshes) {
    if(e.second->getCastShadow()) {
      const DrawDataList& dds = e.second->getDrawData();
      allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
    }
  }

  for(DrawDataList::value_type& dd : allToDraw) {
    dd.SP = dd.SSP;
  }

  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_sunViewMatrix"] = m_sunCamera.viewMatrix();
  m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_sunViewMatrix"] = m_sunCamera.viewMatrix();
  m_sceneSpecificShaderUniforms.Vec3Uniforms["u_directionalLight.direction"] = m_sun.direction();
  m_sceneSpecificShaderUniforms.Vec4Uniforms["u_directionalLight.diffuse"] = m_sun.diffuse();
  m_sceneSpecificShaderUniforms.Vec4Uniforms["u_directionalLight.ambient"] = m_sun.ambient();
  m_sceneSpecificShaderUniforms.FloatUniforms["u_fogDensity"] = m_sky.FogDensity;

  for(int shadowPass = 0; shadowPass < m_shadowMapping.getNumberOfPasses(); shadowPass++)
  {
    m_shadowBufferTarget[shadowPass]->bind();
    context->setCullFace(RenderContext::CullFace::Front);
    context->clear();



    m_sceneSpecificShaderUniforms.Matrix4Uniforms["u_shadowProjectionMatrix"] = m_shadowMapping.getShadowMapProjections()[shadowPass];


    context->draw(allToDraw, m_sceneSpecificShaderUniforms);
    m_shadowBufferTarget[shadowPass]->release();
  }


}



void Scene::draw(RenderContext* context) {



  m_shadowMapping.updateShadowPass(m_camera, m_sunCamera);


  drawShadowPass(context);



  context->setCullFace(RenderContext::CullFace::Back);
  context->setViewport(context->width(), context->height());

  DrawDataList allToDraw;
  for(Entities::value_type e : m_meshes) {
    const DrawDataList& dds = e.second->getDrawData();
    allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
  }

  for(const DebugBoxEntities::value_type e : m_debugBoxes) {
    allToDraw.push_back(e.second->getDrawData());
  }


  std::vector<unsigned int> shadowMap;
  for(int i = 0; i < m_shadowMapping.getNumberOfPasses(); ++i) {
    shadowMap.push_back(4+i);
  }

  for(DrawDataList::value_type& dd : allToDraw) {

    for(int i = 0; i < m_shadowMapping.getNumberOfPasses(); ++i) {
      dd.TEX[4+i] = m_shadowBufferTarget[i]->getDepthTexture();
    }
  }
  DrawData skyDd = m_sky.getDrawData();

  allToDraw.push_back(skyDd);

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

  context->draw(allToDraw, m_sceneSpecificShaderUniforms);


}


}
