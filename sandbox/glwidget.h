#pragma once

#include <renderer_engine/widget.h>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>

#include <renderer_engine/renderdevice.h>
#include <renderer_engine/rendercontext.h>
#include "models/globalsettingsmodel.h"

namespace sa {
class MeshRenderable;
class Scene;
typedef std::shared_ptr<Scene> ScenePtr;
}
class GLWidget : public sa::Widget, protected QOpenGLFunctions
{
public:
  GLWidget(QWidget* parent = nullptr);
  ~GLWidget();

  void setModel(GlobalSettingsModelPtr globalSettingsModel);
  // QOpenGLWidget interface
protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  void onMouseMove(MouseEvent event);
  void onKeyDown(KeyEvent event);
  void onKeyUp(KeyEvent event);
private:
  GlobalSettingsModelPtr m_globalSettingsModel;

  QOpenGLDebugLogger *m_debugLogger = nullptr;

  sa::RenderDevice renderDevice;
  sa::RenderContext renderContext;

  sa::ScenePtr m_scene;
  static const int NUMKEYS = 1024;
  bool keys[NUMKEYS];

  QVector2D mouseDelta;


  float currentTime = 0.0;
  bool firstTime = true;
  float dt = 1/60.0;

  // QWidget interface
protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;
};

