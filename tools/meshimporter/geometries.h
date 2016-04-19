#ifndef GEOMETRIES_H
#define GEOMETRIES_H
#include <QDir>
class Geometries
{
public:
  static bool groundPlaneToXML(bool toXML, unsigned int width, unsigned int height, const QDir& destDir);

};

#endif // GEOMETRIES_H
