#include "glwidget.h"
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/rendercontext.h>
#include <renderer_engine/vertexdescription.h>
#include <scene_engine/meshrenderable.h>
#include <scene_engine/fpscamera.h>
#include <scene_models/meshmodel.h>
#include <QTimer>
#include <QOpenGLTexture>
#include <renderer_engine/drawdata.h>
#include <scene_engine/scene.h>
#include <deque>

sa::RenderDevice renderDevice;
sa::RenderContext renderContext;
sa::VertexArrayPtr vertexArray;
sa::ShaderProgramPtr shaderProgram;

sa::MeshRenderable* meshRenderable;
std::deque<sa::DrawData> drawDataDeque;

sa::Scene* scene;

struct Vertex {
  Vertex(float xx, float yy, float zz, float rr, float gg, float bb, float aa)
    : x(xx)
    , y(yy)
    , z(zz)
    , r(rr)
    , g(gg)
    , b(bb)
    , a(aa ){ }

  float x = 0, y = 0, z;
  float r, g, b, a;
} *vertices;



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



void GLWidget::initializeGL() {
  m_debugLogger = new QOpenGLDebugLogger(this);
  if (m_debugLogger->initialize())
  {
//    qDebug() << "GL_DEBUG Debug Logger" << m_debugLogger << "\n";
//    connect(m_debugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(messageLogged(QOpenGLDebugMessage)));
//    m_debugLogger->startLogging();
  }

  scene = new sa::Scene(this->width(), this->height());
  scene->camera().setEye({0,50,200});
  scene->setSun(sa::DirectionalLight(
  {0,1,1},
  {0.5,0.5, 0.5, 1},
  {1.0,1.0, 1.0, 1}));

//    scene->addMeshEntity("desert_city", "desert_city.xml");
  scene->addMeshEntity("groundplane100x100", "groundplane100x100.xml");
  scene->addMeshEntity("bob0", "bob.xml");
  scene->getMeshEntity("bob0")->playSkeletalAnimation("");
  scene->addMeshEntity("motioncapture0", "motioncapture.xml");
  scene->getMeshEntity("motioncapture0")->playNodeAnimation("");
  scene->getMeshEntity("motioncapture0")->setPosition(120,0,40);
  float LO = -200.0f;
  float HI = 200.0f;

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
//    scene->addMeshEntity(meshName, "/home/tobias/source/spiralarchitect/data/meshes/", "motioncapture.xml");

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

void GLWidget::resizeGL(int , int ) {
  renderContext.makeDirty();
}

float currentTime = 0.0;
bool firstTime = true;
void GLWidget::paintGL(){
  if(firstTime) {
    renderContext.create(this->defaultFramebufferObject(), this->width(), this->height());
    scene->initialize(&renderDevice);
    firstTime = false;
  }
  renderContext.clear();

  scene->toGPU(&renderDevice, &renderContext);
  scene->update(1/60.0f);
  scene->toCPU();

  scene->draw(&renderContext);
  currentTime+=1/60.0;

}

void GLWidget::onMouseMove(MouseEvent event) {
  scene->camera().rotate(-event.dx/500.0f, -event.dy/500.0f, 0);
}

void GLWidget::onKeyDown(KeyEvent event) {
  if(event.keyCode == 0x57)
    scene->camera().moveForward(0.5);
  if(event.keyCode == 0x53)
    scene->camera().moveForward(-0.5);
  if(event.keyCode == 65)
    scene->camera().moveRight(-0.5);
  if(event.keyCode == 68)
    scene->camera().moveRight(0.5);
  if(event.keyCode == 76)
    scene->m_createLightFrustum = true;
  //qDebug() << "key: " << event.keyCode;

}

void GLWidget::onKeyUp(KeyEvent /*event*/) {

}
