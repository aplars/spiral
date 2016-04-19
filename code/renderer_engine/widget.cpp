#include "widget.h"
#include <QDebug>
#include <QMouseEvent>

namespace sa {
Widget::Widget(QWidget* parent)
  : QOpenGLWidget(parent)
{
  setFocusPolicy(Qt::StrongFocus);
//  QSurfaceFormat format;
//  format.setDepthBufferSize(24);
//  format.setVersion(3, 1);
//  setFormat(format);
}

Widget::~Widget()
{

}

void Widget::mousePressEvent(QMouseEvent *event) {
  m_lastPos = event->pos();
}

void	Widget::mouseMoveEvent(QMouseEvent * event) {
  MouseEvent  e;
  e.x = event->x();
  e.y = event->y();
  e.dx = event->x() - m_lastPos.x();
  e.dy = event->y() - m_lastPos.y();

  onMouseMove(e);
  m_lastPos = event->pos();
}

void Widget::keyPressEvent(QKeyEvent * event) {
  KeyEvent e;
  e.keyCode = event->key();
  onKeyDown(e);
}

void Widget::keyReleaseEvent(QKeyEvent * event) {
  KeyEvent e;
  e.keyCode = event->key();
  onKeyUp(e);
}

}

