#include "geometries.h"
#include <scene_models/meshmodel.h>
#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

bool Geometries::groundPlaneToXML(bool toXML, unsigned int width, unsigned int height, const QDir& destDir) {
  bool isOk = true;
  sa::MeshModel saModel = sa::MeshModel::createGroundPlane(width, height);

  //Save the header
  {
    std::string destDirString = (destDir.absolutePath().toStdString())+".header";
    std::ofstream ofs(destDirString.c_str());
    if(ofs.good())
    {
      if(toXML)
      {
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(saModel.m_header);
      }
      else {
        boost::archive::binary_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(saModel.m_header);
      }
    }
    else {
      isOk = false;
    }
  }
  //Save the data
  {
    std::string destDirString = (destDir.absolutePath().toStdString())+".data";
    std::ofstream ofs(destDirString.c_str());
    if(ofs.good())
    {
      if(toXML)
      {
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(saModel.m_data);
      }
      else
      {
        boost::archive::binary_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(saModel.m_data);
      }
    }
    else {
      isOk = false;
    }
  }
  return isOk;
}
