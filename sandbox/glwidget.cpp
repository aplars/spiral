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
#include <QDebug>
#include <QMimeData>
#include <QDragEnterEvent>

#include "models/entities_tree_model/entityfactorymodel.h"

GLWidget::GLWidget(QWidget* parent)
  : sa::Widget(parent)
{ }

GLWidget::~GLWidget()
{
  this->makeCurrent();
  renderContext.destroy();
}


void GLWidget::setModel(GlobalSettingsModelPtr globalSettingsModel) {
  m_globalSettingsModel = globalSettingsModel;
  m_globalSettingsModel->PropertyChanged += [this](const std::string&) {
    m_scene->setTime(m_globalSettingsModel->julianDay(), m_globalSettingsModel->timeOfDay());
    m_scene->setAtmosphereFogDensity(m_globalSettingsModel->fogDensity());
    m_scene->setAtmosExposure(m_globalSettingsModel->atmosExposure());
    m_scene->setAtmosDecay(m_globalSettingsModel->atmosDecay());
    m_scene->setAtmosDensity(m_globalSettingsModel->atmosDensity());
    m_scene->setAtmosWeight(m_globalSettingsModel->atmosWeight());
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

  sa::Config config;
  config.init("sa_config.conf");
  m_scene = sa::ScenePtr(new sa::Scene(this->width(), this->height(), config));
  m_scene->camera().setEye({0,50,200});





  sa::MeshRenderablePtr bobMesh;
  bobMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "bob.xml"));


//  sa::MeshRenderablePtr motioncaptureMesh;
//  motioncaptureMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "motioncapture.xml"));

  sa::MeshRenderablePtr landscapeMesh;
  landscapeMesh.reset(new sa::MeshRenderable(config.getParam("DATA_DIR") + "/meshes/", "landscape.xml"));

  m_scene->addMeshEntity("landscape", landscapeMesh, false);

  m_scene->addMeshEntity("bob0", bobMesh, true);
  m_scene->getMeshEntity("bob0")->playSkeletalAnimation("");

//  m_scene->addMeshEntity("motioncaptureLeft", motioncaptureMesh, true);
//  m_scene->getMeshEntity("motioncaptureLeft")->playNodeAnimation("");
//  m_scene->getMeshEntity("motioncaptureLeft")->setPosition(-120,0,40);



  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(16);


}

void GLWidget::resizeGL(int w, int h) {
  renderContext.makeDirty();
  m_scene->resize(w, h);
}


void GLWidget::paintGL(){

  if(firstTime) {
    renderContext.create(this->defaultFramebufferObject(), this->width(), this->height());
    firstTime = false;
  }
  renderContext.clear();
  renderContext.setViewport(width(), height());


  if(keys[0x57])
    m_scene->camera().moveForward(20*dt);
  if(keys[0x53])
    m_scene->camera().moveForward(-20*dt);
  if(keys[65])
    m_scene->camera().moveRight(20*dt);
  if(keys[68])
    m_scene->camera().moveRight(-20*dt);

  m_scene->camera().rotate(mouseDelta.x(), mouseDelta.y(), 0);

  m_scene->toGPU(&renderDevice, &renderContext);
  m_scene->update(dt);
  m_scene->toCPU();

  m_scene->draw(&renderContext);
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

void GLWidget::dragEnterEvent(QDragEnterEvent *event)
{
  qDebug() << "dragEnterEvent";

  const QMimeData* mimeData = event->mimeData();

  QByteArray data = mimeData->data(sa::EntityFactoryModel::MimeType);
  if(!data.isEmpty() && !data.isNull())
  {
    std::stringstream strstream;
    strstream.write(data.constData(), data.size());
    if(strstream.good())
    {
      int npathtojson;
      strstream >> npathtojson;
      char * buffer = new char [npathtojson+1];

      strstream.read(buffer, npathtojson);
      buffer[npathtojson] = '\0';
      std::string pathToJson(buffer);
      int nnameofjson;
      strstream >> nnameofjson;

      std::string nameOfJson;
      strstream >> nameOfJson;

      nlohmann::json json;
      std::ifstream thestream((pathToJson + nameOfJson).c_str());
      json = json.parse(thestream);
      sa::EntityFactoryModel* model = new sa::EntityFactoryModel(pathToJson);
      model->fromJson(json);

      glm::vec3 pickOrigin;
      glm::vec3 pickDir;
      m_scene->getPickRay(event->pos().x(), event->pos().y(), pickOrigin, pickDir);

//      float dist;

//      bool didIntersect = glm::intersectRayPlane(pickOrigin, pickDir, glm::vec3(), glm::vec3(0.0f, 0.0f, -1.0f), dist);

//      glm::vec3 pos = pickOrigin + pickDir * dist;

//      this->makeCurrent();
//      m_actorToCreateModel = new ActorModel(model);
//      m_actorToCreateModel->setPosition(pos);
//      m_actorSpriteViewToCreate.reset(new ActorSpriteView());
//      m_actorSpriteViewToCreate->setModel(m_actorToCreateModel);
//      m_sceneView.addRenderable(m_actorSpriteViewToCreate);
    }
  }

}

void GLWidget::dragMoveEvent(QDragMoveEvent *event)
{
}

void GLWidget::dropEvent(QDropEvent *event)
{
}
