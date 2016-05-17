#include "mainwindow.h"
#include <QApplication>

#include "assimptosamodels.h"
#include "geometries.h"
#include <config/config.h>
#include <QDebug>
//
int main(int argc, char *argv[]) {
  Q_UNUSED(argc);
  Q_UNUSED(argv);

  sa::ConfigurationManager config;
  bool isOk = config.init("sa_config.conf");
  if(isOk) {
  qDebug() << "config OK";
  std::string dataDir = config.getParam("DATA_DIR");
  QString qDataDir(dataDir.c_str());
  qDebug() << "Using dataDir: " << qDataDir;
  /*
  Geometries::groundPlaneToXML(
        false,
        1000,
        1000,
        QDir(qDataDir + "/meshes/groundplane100x100.xml"));

  AssimpToSAModels::convertToXML(
        false,
        QDir(qDataDir + "/import_models/models-nonbsd/MD5/Bob.md5mesh"),
        QDir(qDataDir + "/meshes/bob.xml"),
        1);


  AssimpToSAModels::convertToXML(
        false,
        QDir(qDataDir + "/import_models/models/ASE/MotionCaptureROM.ase"),
        QDir(qDataDir + "/meshes/motioncapture.xml"),
        1);
        */
  AssimpToSAModels::convertToXML(
        false,
        QDir(qDataDir + "/import_models/models/OBJ/desert city.obj"),
        QDir(qDataDir + "/meshes/desert_city.xml"),
        1);
  }
  return 0;
}