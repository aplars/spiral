#ifndef SAENGINEVIEW_H
#define SAENGINEVIEW_H

#include <QQuickItem>

class SaEngineView : public QQuickItem
{
  Q_OBJECT
public:
  SaEngineView();

signals:

public slots:
  void handleWindowChanged(QQuickWindow *win);
  void sync();
  void beforeRendering();

private:
  bool m_beforeRenderingConnected = false;
  QSize m_viewportSize;
  QPoint m_viewportPosition;
};

#endif // SAENGINEVIEW_H
