#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <renderer_engine/widget.h>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>

#include <renderer_engine/renderdevice.h>
#include <renderer_engine/rendercontext.h>

namespace sa {
class MeshRenderable;
class Scene;
}
class GLWidget : public sa::Widget, protected QOpenGLFunctions
{
public:
  GLWidget(QWidget* parent = nullptr);
  ~GLWidget();

  // QOpenGLWidget interface
protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();


  void onMouseMove(MouseEvent event);
  void onKeyDown(KeyEvent event);
  void onKeyUp(KeyEvent event);
private:
  QOpenGLDebugLogger *m_debugLogger = nullptr;

  sa::RenderDevice renderDevice;
  sa::RenderContext renderContext;

  sa::Scene* scene;
  static const int NUMKEYS = 1024;
  bool keys[NUMKEYS];

  QVector2D mouseDelta;


  float currentTime = 0.0;
  bool firstTime = true;
  float dt = 1/60.0;
};

#endif // GLWIDGET_H
