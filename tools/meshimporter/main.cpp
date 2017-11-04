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

  sa::Config config;
  bool isOk = config.init("sa_config.conf");
  if(isOk) {
  qDebug() << "config OK";
  std::string dataDir = config.getParam("DATA_DIR");
  QString qDataDir(dataDir.c_str());
  qDebug() << "Using dataDir: " << qDataDir;

//  Geometries::groundPlaneToXML(
//        false,
//        1000,
//        1000,
//        QDir(qDataDir + "/meshes/groundplane100x100.xml"));

//  AssimpToSAModels::convertToXML(
//        false,
//        QDir(qDataDir + "/import_models/models/Collada/silent+hill+2+-+tangstelle/model.dae"),
//        QDir(qDataDir + "/meshes/silenthill.xml"),
//        false,
//        200);


  AssimpToSAModels::convertToXML(
        false,
        QDir(qDataDir + "/rawmeshes/md5/bob/Bob.md5mesh"),
        QDir(qDataDir + "/meshes/bob.xml"),
        false,
        1);


  AssimpToSAModels::convertToXML(
        false,
        QDir(qDataDir + "/rawmeshes/ase/motioncapture/MotionCaptureROM.ase"),
        QDir(qDataDir + "/meshes/motioncapture.xml"),
        false,
        1);

//  AssimpToSAModels::convertToXML(
//        false,
//        QDir(qDataDir + "/import_models/models/OBJ/desert city.obj"),
//        QDir(qDataDir + "/meshes/desert_city.xml"),
//        true,
//        3);

//  AssimpToSAModels::convertToXML(
//        false,
//        QDir(qDataDir + "/import_models/models/Collada/Fortezza/model.dae"),
//        QDir(qDataDir + "/meshes/Fortezza.xml"),
//        true,
//        1);

  AssimpToSAModels::convertToXML(
        false,
        QDir(qDataDir + "/rawmeshes/obj/landscape/landscape.obj"),
        QDir(qDataDir + "/meshes/landscape.xml"),
        true,
        2000);
  }
  qDebug() << "DONE! ";
  return 0;

}
