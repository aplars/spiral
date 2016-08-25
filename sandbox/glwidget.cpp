#include "glwidget.h"
#include <renderer_engine/vertexdescription.h>
#include <scene_engine/meshrenderable.h>
#include <scene_engine/fpscamera.h>
#include <scene_models/meshmodel.h>
#include <QTimer>
#include <QOpenGLTexture>
#include <renderer_engine/drawdata.h>
#include <scene_engine/scene.h>
#include <deque>



//struct Vertex {
//  Vertex(float xx, float yy, float zz, float rr, float gg, float bb, float aa)
//    : x(xx)
//    , y(yy)
//    , z(zz)
//    , r(rr)
//    , g(gg)
//    , b(bb)
//    , a(aa ){ }

//  float x = 0, y = 0, z;
//  float r, g, b, a;
//} *vertices;



GLWidget::GLWidget(QWidget* parent)
  : sa::Widget(parent)
{ }

GLWidget::~GLWidget()
{
  this->makeCurrent();
  delete scene;
  scene = nullptr;

  renderContext.destroy();
}


void GLWidget::setModel(GlobalSettingsModel* globalSettingsModel) {
  m_globalSettingsModel = globalSettingsModel;
  m_globalSettingsModel->PropertyChanged += [this](const std::string&) {
    scene->setTime(m_globalSettingsModel->julianDay(), m_globalSettingsModel->timeOfDay());
    scene->setAtmosphereFogDensity(m_globalSettingsModel->fogDensity());
    scene->setUseStableShadowMapping(m_globalSettingsModel->useStableShadowMapping());
  };
}

void GLWidget::initializeGL() {
  for(int i = 0; i < NUMKEYS; ++i) {
    keys[i] = false;
  }

  m_debugLogger = new QOpenGLDebugLogger(this);
  if (m_debugLogger->initialize())
  {
//    qDebug() << "GL_DEBUG Debug Logger" << m_debugLogger << "\n";
//    connect(m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
//    m_debugLogger->startLogging();
  }

  scene = new sa::Scene(this->width(), this->height());
  scene->camera().setEye({0,50,200});
//  scene->setSun(sa::DirectionalLight(
//  {1,1,1},
//  {0.75,0.75, 0.75, 1},
//  {0.2,0.2, 0.2, 1}));


  sa::ConfigurationManager config;
  config.init("sa_config.conf");


  sa::MeshRenderablePtr fortezza;
  fortezza.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "Fortezza.xml"));

  sa::MeshRenderablePtr silenthill;
  silenthill.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "silenthill.xml"));

  sa::MeshRenderablePtr bobMesh;
  bobMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "bob.xml"));

  sa::MeshRenderablePtr groundMesh;
  groundMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "groundplane100x100.xml"));

  sa::MeshRenderablePtr motioncaptureMesh;
  motioncaptureMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "motioncapture.xml"));

  sa::MeshRenderablePtr landscapeMesh;
  landscapeMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "landscape.xml"));



//  scene->addMeshEntity("silenthill", silenthill);
  scene->addMeshEntity("landscape", landscapeMesh, false);
 // scene->addMeshEntity("desert_city", "desert_city.xml");

//  scene->addMeshEntity("fortezza", fortezza);
  //scene->addMeshEntity("groundplane100x100", groundMesh);


  scene->addMeshEntity("bob0", bobMesh, true);
  scene->getMeshEntity("bob0")->playSkeletalAnimation("");

//  scene->addMeshEntity("bob1", bobMesh);
//  scene->getMeshEntity("bob1")->setPosition(120,0,40);

  scene->addMeshEntity("motioncaptureLeft", motioncaptureMesh, true);
  scene->getMeshEntity("motioncaptureLeft")->playNodeAnimation("");
  scene->getMeshEntity("motioncaptureLeft")->setPosition(-120,0,40);

  //scene->addMeshEntity("motioncaptureRight", motioncaptureMesh);
  //scene->getMeshEntity("motioncapture1")->playNodeAnimation("");
  //scene->getMeshEntity("motioncaptureRight")->setPosition(120,0,40);
//  float LO = -200.0f;
//  float HI = 200.0f;


//  float LO_H = 0.0f;
//  float HI_H = 3.1415*2;

//  for(int i = 0; i < 20; i++) {
//    std::string meshName = "bob" + std::to_string(i);
//    scene->addMeshEntity(meshName, "bob.xml");

//    float x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
//    float y = 0;//LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
//    float z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
//    //float h = LO_H + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI_H-LO_H)));

//    scene->getMeshEntity(meshName)->setPosition(sa::Vector3T<float>(x, y, z));
//    //scene->getMeshEntity(meshName)->setHeading(h);
//    scene->getMeshEntity(meshName)->playSkeletalAnimation("");
//  }


//  for(int i = 0; i < 20; i++) {
//    std::string meshName = "motioncapture" + std::to_string(i);
//    scene->addMeshEntity(meshName, "motioncapture.xml");

//    float x = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
//    float y = 0;//LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
//    float z = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));

//    scene->getMeshEntity(meshName)->setPosition(sa::Vector3T<float>(x, y, z));
//    scene->getMeshEntity(meshName)->playNodeAnimation("");
//  }


  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(16);
}

void GLWidget::resizeGL(int w, int h) {
  renderContext.makeDirty();
}


void GLWidget::paintGL(){

  if(firstTime) {
    renderContext.create(this->defaultFramebufferObject(), this->width(), this->height());
    firstTime = false;
  }
  renderContext.clear();
  renderContext.setViewport(width(), height());


  if(keys[0x57])
    scene->camera().moveForward(20*dt);
  if(keys[0x53])
    scene->camera().moveForward(-20*dt);
  if(keys[65])
    scene->camera().moveRight(20*dt);
  if(keys[68])
    scene->camera().moveRight(-20*dt);

  scene->camera().rotate(mouseDelta.x(), mouseDelta.y(), 0);

  scene->toGPU(&renderDevice, &renderContext);
  scene->update(dt);
  scene->toCPU();

  scene->draw(&renderContext);
  currentTime+=dt;

  mouseDelta.setX(0);
  mouseDelta.setY(0);

}

void GLWidget::onMouseMove(MouseEvent event) {
  mouseDelta.setX(-event.dx/500.0f);
  mouseDelta.setY(-event.dy/500.0f);
}

void GLWidget::onKeyDown(KeyEvent event) {
  keys[event.keyCode] = true;

}

void GLWidget::onKeyUp(KeyEvent event) {
  keys[event.keyCode] = false;
}
