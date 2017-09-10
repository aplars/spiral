#include "saengineview.h"
#include <QQuickWindow>

SaEngineView::SaEngineView()
{

  connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));



}

void SaEngineView::handleWindowChanged(QQuickWindow *win)
{

//  m_win = win;
  if (win) {
    connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
    //    //connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
    //    // If we allow QML to do the clearing, they would clear what we paint
    //    // and nothing would show.
    //win->setClearBeforeRendering(false);
  }
}

void SaEngineView::sync()
{
  if(!m_beforeRenderingConnected) {
    m_beforeRenderingConnected = true;
    QObject::connect(window(), SIGNAL(beforeRendering()), this, SLOT(beforeRendering()), Qt::DirectConnection);
  }
  //m_viewportSize = window()->size() * window()->devicePixelRatio();
  m_viewportSize = QSize(boundingRect().width(), boundingRect().height());
  m_viewportPosition = QPoint(boundingRect().x(), boundingRect().y());

}


void SaEngineView::beforeRendering() {
  glViewport(m_viewportPosition.x(), m_viewportPosition.y(), m_viewportSize.width(), m_viewportSize.height());
  glEnable(GL_SCISSOR_TEST);
  glScissor(m_viewportPosition.x(), m_viewportPosition.y(), m_viewportSize.width(), m_viewportSize.height());
  qDebug() << m_viewportSize;
  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  window()->resetOpenGLState();
}
