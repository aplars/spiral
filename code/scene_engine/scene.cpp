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
  : m_sun({1,0,1}, {0.5,0.5, 0.5, 1}, {1.0,1.0, 1.0, 1})
  , m_imageCache(1000)
  , m_textureCache(1000)
  , m_shaderCache(1000)
{

  m_shadowMapCascadeDistance.push_back(100);
  m_shadowMapCascadeDistance.push_back(125);
  m_shadowMapCascadeDistance.push_back(150);
  m_shadowMapCascadeDistance.push_back(200);


  float aspect = width/static_cast<float>(height);
  if(height > width)
    aspect = height/static_cast<float>(width);
  m_projection = sa::Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0f), aspect, 1, m_shadowMapCascadeDistance.back());

  for(float dist : m_shadowMapCascadeDistance)
  {
    m_shadowProjections.push_back(sa::Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0f), aspect, 1, dist));
  }
  m_config.init("sa_config.conf");
}

void Scene::initialize(RenderDevice* device) {

}

void Scene::setSun(const DirectionalLight& sun) {
  m_sun = sun;
}

void Scene::addMeshEntity(const std::string& name, const std::string& resourceName) {
  m_meshes[name] = new StreamedMeshEntity(m_config.getParam("DATA_DIR") + "/meshes/", resourceName);
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
  bool allVisibleObjectsBboxSet = false;
  AABBModel allVisibleObjectsBbox;
  std::array<PlaneT<float>, 6> frustum = m_camera.getFrustum(m_projection);
  for(Entities::value_type e : m_meshes) {
    AABBModel bbox = e.second->getBoundingBox();
    IntersectionTests::Side side = IntersectionTests::FrustumAABBIntersect(frustum, bbox.getMin()-Vector3T<float>(10, 10, 10), bbox.getMax()+Vector3T<float>(10, 10, 10));
    if(side == IntersectionTests::Inside || side == IntersectionTests::Intersect) {
      if(!allVisibleObjectsBboxSet)
      {
        allVisibleObjectsBboxSet = true;
        allVisibleObjectsBbox = bbox;
      }
      else
      {
        allVisibleObjectsBbox.expand(bbox);
      }
      if(e.second->currentDataStorage() == DataStorage::Disk)
      {
        //We don not want to load this sucker again therefore we set it as pending.
        e.second->setPendingStorage();
        BackgroundWorkPtr work;
        work.reset(new BackgroundWork(e.second));
        work->doWork = [=](StreamedMeshEntity* entity) {
          entity->toCPU(m_imageCache);
        };
        work->workDone = [](StreamedMeshEntity* /*entity*/) {
        };

        m_meshLoader.push(work);
      }
    }
    else {
      if(e.second->currentDataStorage() == DataStorage::GPU) {
        e.second->setPendingStorage();
        e.second->unloadGPU();
        e.second->unloadCPU();
        e.second->setDiskStorage();
      }
    }
  }

  Vector3T<float> centerpoint = m_camera.getFrusumCenterPoint(m_projection);

  m_sunCamera.setLookAt(centerpoint + m_sun.direction(), centerpoint, Vector3T<float>(0, 1, 0));
}

void Scene::toGPUOnce(RenderDevice* device, RenderContext* context) {
  if(m_firstTimeInToGPU) {
    if(m_shadowBufferTarget.size() <= 0) {
      for(int i = 0; i < m_shadowMapCascadeDistance.size(); i++) {
        m_shadowBufferTarget.push_back(context->createRenderDepthToTexture(context->width(), context->height()));
      }
    }

    if(!m_shadowBufferRectangle) {
      createShadowBufferRectangle(device, context, 0, 0, context->width()/4, context->height()/4);
    }
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
      e.second->toGPU(m_config, m_shadowMapCascadeDistance.size(), m_textureCache, m_shaderCache, device, context);
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


#pragma omp parallel for
  for(unsigned int i = 0; i < entetiesDeq.size(); ++i) {
    StreamedMeshEntity* e = entetiesDeq[i];
    e->applyAnimations(dt);
  }
#pragma omp parallel for
  for(unsigned int i = 0; i < entetiesDeq.size(); ++i) {
    StreamedMeshEntity* e = entetiesDeq[i];
    if(e->currentDataStorage() == DataStorage::GPU)
      e->applyTransformations();
  }

  m_currentTime+=dt;
}
#include <scene_models/aabbmodel.h>

void Scene::drawShadowPass(RenderContext* context) {

  m_depthBiasMVPMatrix.clear();
  //unsigned int shadowPass = 0;
  for(int shadowPass = 0; shadowPass < m_shadowMapCascadeDistance.size(); shadowPass++)
  {
    context->setCullFace(RenderContext::CullFace::Front);
    m_shadowBufferTarget[shadowPass]->bind();
    context->clear();

    std::deque<DrawData> allToDraw;
    for(Entities::value_type e : m_meshes) {
      const std::deque<DrawData>& dds = e.second->getDrawData();
      allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
    }





    std::array<Vector3T<float>, 8> frustumpoints = m_camera.getFrusumPoints(m_shadowProjections[shadowPass]);

    Matrix44T<float> shadowCamView = m_sunCamera.viewMatrix();

    Vector3T<float> min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector3T<float> max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

    for(int i = 0; i < 8; ++i) {
      Vector4T<float> vv = shadowCamView.Vec3Transform(frustumpoints[i]);
      for(int j = 0; j < 3; ++j) {
        min[j] = Min(vv[j], min[j]);
        max[j] = Max(vv[j], max[j]);
      }
    }


    {
      Vector3T<float> mmin = frustumpoints[0];
      Vector3T<float> mmax = mmin;

      for(int i = 0; i < 8; ++i) {
        Vector3T<float> vv = frustumpoints[i];
        for(int j = 0; j < 3; ++j) {
          if(vv[j] < mmin[j])
            mmin[j] = vv[j];
          if(vv[j] > mmax[j])
            mmax[j] = vv[j];
        }
      }
      if(!m_lightFrustumAabb && m_createLightFrustum)
      {
        m_lightFrustumAabb = new AABBModel();
        *m_lightFrustumAabb = AABBModel::createFromMinMax(mmin, mmax);

        m_createLightFrustum = false;
      }
    }

    Matrix44T<float> ortho = Matrix44T<float>::GetOrthographicProjection(
          min.X(), max.X(), min.Y(), max.Y(), -max.Z(), -min.Z()
          );

    static Matrix44T<float> biasMatrix(
          0.5, 0.0, 0.0, 0.5,
          0.0, 0.5, 0.0, 0.5,
          0.0, 0.0, 0.5, 0.5,
          0.0, 0.0, 0.0, 1.0);


    m_depthBiasMVPMatrix.push_back(biasMatrix * ortho * m_sunCamera.viewMatrix());
    for(std::deque<DrawData>::value_type& dd : allToDraw) {
      dd.SP = dd.SSP;

      dd.Matrix4Uniforms[dd.SP->uniformLocation("u_viewMatrix")] = m_sunCamera.viewMatrix();
      dd.Matrix4Uniforms[dd.SP->uniformLocation("u_projectionMatrix")] = ortho;

//      dd.Vec3Uniforms[dd.SP->uniformLocation("u_directionalLight.direction")] = m_sun.direction();
//      dd.Vec4Uniforms[dd.SP->uniformLocation("u_directionalLight.ambient")] = m_sun.ambient();
//      dd.Vec4Uniforms[dd.SP->uniformLocation("u_directionalLight.diffuse")] = m_sun.diffuse();

//      dd.Vec3Uniforms[dd.SP->uniformLocation("u_eyePosition")] = m_sunCamera.eye();

    }
    for(std::deque<DrawData>::value_type& dd : allToDraw) {
      context->draw(dd);
    }
    m_shadowBufferTarget[shadowPass]->release();
  }
}



void Scene::draw(RenderContext* context) {
  drawShadowPass(context);
  context->setCullFace(RenderContext::CullFace::Back);
  context->setViewport(context->width(), context->height());

  std::deque<DrawData> allToDraw;
  for(Entities::value_type e : m_meshes) {
    const std::deque<DrawData>& dds = e.second->getDrawData();
    allToDraw.insert(allToDraw.end(), dds.begin(), dds.end());
  }

  for(const DebugBoxEntities::value_type e : m_debugBoxes) {
    allToDraw.push_back(e.second->getDrawData());
  }


  for(std::deque<DrawData>::value_type& dd : allToDraw) {
    dd.Matrix4Uniforms[dd.SP->uniformLocation("u_viewMatrix")] = m_camera.viewMatrix();
    dd.Matrix4Uniforms[dd.SP->uniformLocation("u_projectionMatrix")] = m_projection;
    dd.Matrix4ArrayUniforms[dd.SP->uniformLocation("u_depthBiasMVPMatrix")] = m_depthBiasMVPMatrix;

    dd.Vec3Uniforms[dd.SP->uniformLocation("u_directionalLight.direction")] = m_sun.direction();
    dd.Vec4Uniforms[dd.SP->uniformLocation("u_directionalLight.ambient")] = m_sun.ambient();
    dd.Vec4Uniforms[dd.SP->uniformLocation("u_directionalLight.diffuse")] = m_sun.diffuse();

    dd.Vec3Uniforms[dd.SP->uniformLocation("u_eyePosition")] = m_camera.eye();


    dd.FloatArrayUniforms[dd.SP->uniformLocation("u_shadowMapCascadeDistance")] = m_shadowMapCascadeDistance;

    for(int i = 0; i < m_shadowMapCascadeDistance.size(); ++i) {
      dd.TEX[3+i] = m_shadowBufferTarget[i]->getDepthTexture();
    }
    std::vector<unsigned int> shadowMap;
    for(int i = 0; i < m_shadowMapCascadeDistance.size(); ++i) {
      shadowMap.push_back(3+i);
    }
    dd.Sampler2DArrayUniforms[dd.SP->uniformLocation("u_shadowMap")] = shadowMap;
  }
  for(std::deque<DrawData>::value_type& dd : allToDraw) {
    context->draw(dd);
  }


  if(m_shadowBufferRectangle)
  {
    m_shadowBufferRectangle->TEX[0] = m_shadowBufferTarget[0]->getDepthTexture();
    m_shadowBufferRectangle->Sampler2DUniforms[m_shadowBufferRectangle->SP->uniformLocation("u_texture")] = 0;
    context->draw(*m_shadowBufferRectangle);
  }
}

void Scene::createShadowBufferRectangle(RenderDevice* device, RenderContext* context, float posx, float posy, float sw, float sh) {
  static const char *vertexShaderSource =
      "attribute highp vec3 posAttr;\n"
      "attribute highp vec2 texAttr;\n"
      "varying vec2 v_texAttr;\n"
      "uniform highp mat4 u_modelMatrix;\n"
      "uniform highp mat4 u_viewMatrix;\n"
      "uniform highp mat4 u_projectionMatrix;\n"
      "void main(void)\n"
      "{\n"
      "    v_texAttr = texAttr;\n"
      "    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(posAttr, 1);\n"
      "}\n";

  static const char *fragmentShaderSource =
      "varying vec2 v_texAttr;\n"
      "uniform sampler2D u_texture;\n"
      "void main(void)\n"
      "{\n"
      "    vec4 tex = texture2D(u_texture, v_texAttr);\n"
      "    gl_FragColor = tex;\n"
      "}\n";

  float vertices[] {
    // front
    posx, posy,  0.0f, 0, 0,
        posx + sw, posy,  0.0f, 1, 0,
        posx + sw, posy + sh,  0.0f, 1, 1,
        posx, posy + sh,  0.0f, 0, 1
  };

  unsigned int faces[] = {
    // front
    0, 1, 2,
    2, 3, 0,
  };
  VertexBufferPtr vb = device->createVertexBuffer(vertices, 4*5*sizeof(float));

  std::set<std::string> defines;
  ShaderProgramPtr sp = device->createShaderProgram(
        vertexShaderSource,
        fragmentShaderSource,
        defines);
  int posAttr = sp->attributeLocation("posAttr");
  int texAttr = sp->attributeLocation("texAttr");
  VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
  };

  VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);

  IndexBufferPtr ib = device->createIndexBuffer(faces, 2*3);

  m_shadowBufferRectangle.reset(new DrawData());
  m_shadowBufferRectangle->IsAlphaBlended = true;
  m_shadowBufferRectangle->IsTwoSided = true;
  m_shadowBufferRectangle->SP = sp;
  m_shadowBufferRectangle->VAO = vao;
  m_shadowBufferRectangle->IB = ib;
  m_shadowBufferRectangle->Matrix4Uniforms[sp->uniformLocation("u_modelMatrix")] = Matrix44T<float>::GetIdentity();
  m_shadowBufferRectangle->Matrix4Uniforms[sp->uniformLocation("u_viewMatrix")] = Matrix44T<float>::GetIdentity();
  m_shadowBufferRectangle->Matrix4Uniforms[sp->uniformLocation("u_projectionMatrix")] = Matrix44T<float>::GetOrthographicProjection(
        RectangleT<float>(0,0, context->width(), context->height()),
        0,
        5
        );

}

}
