#pragma once

#include <QOpenGLWidget>
namespace sa {
class Widget : public QOpenGLWidget
{
public:
  struct MouseEvent {
    int x;
    int y;
    int dx;
    int dy;
  };

  enum Key {
    Key_W = Qt::Key_W,
    Key_A = Qt::Key_A,
    Key_S = Qt::Key_S,
    Key_D = Qt::Key_D,
  };

  struct KeyEvent {
    int keyCode;
  };

  Widget(QWidget* parent = nullptr);
  virtual ~Widget();

  virtual void onMouseMove(MouseEvent event) = 0;
  virtual void onKeyDown(KeyEvent event) = 0;
  virtual void onKeyUp(KeyEvent event) = 0;
private:
  QPoint m_lastPos;
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent * event);
  void keyPressEvent(QKeyEvent * event);
  void keyReleaseEvent(QKeyEvent * event);
};
}
