#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include "adt/lru.h"
#include <iostream>



int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  QSurfaceFormat fmt;
  fmt.setDepthBufferSize(24);
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
      fmt.setVersion(3, 3);
      fmt.setProfile(QSurfaceFormat::CompatibilityProfile);
  } else {
      fmt.setVersion(3, 0);
  }
  QSurfaceFormat::setDefaultFormat(fmt);

  int kuken = a.exec();

  return kuken;
}
