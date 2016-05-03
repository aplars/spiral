#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>

int main(int argc, char *argv[])
{

  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  /*
  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
      fmt.setVersion(3, 3);
      fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
  } else {
      fmt.setVersion(3, 0);
  }
  QSurfaceFormat::setDefaultFormat(fmt);
  */
  return a.exec();
}
