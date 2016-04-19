#ifndef GLWIDGET_H
#define GLWIDGET_H
#include <renderer_engine/widget.h>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
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
};

#endif // GLWIDGET_H
