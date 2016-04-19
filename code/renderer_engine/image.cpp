#include "image.h"
namespace sa {
Image::~Image()
{

}


Image::Image(const unsigned char* data, int w, int h)
  : m_qObject(data, w, h, QImage::Format_ARGB32)
{
  m_isNull = m_qObject.isNull();
  //m_qObject = m_qObject.mirrored();
}

Image::Image(const char* fileName)
  : m_qObject(QString(fileName))
{
  m_qObject = m_qObject.mirrored();
  m_qObject = m_qObject.convertToFormat(QImage::Format_ARGB32);
  m_isNull = m_qObject.isNull();
}
}
