#include "image.h"
namespace sa {
Image::~Image()
{}

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

void Image::save(const std::string &fileName) {
  m_qObject.save(fileName.c_str());
}

unsigned int Image::getWidth() const {
  return m_qObject.width();
}

unsigned int Image::getHeight() const {
  return m_qObject.height();
}

Image::Color Image::getPixel(unsigned int x, unsigned y) const {
  QColor color = m_qObject.pixelColor(x, y);
  Color c;
  c.r = color.redF();
  c.g = color.greenF();
  c.b = color.blueF();
  c.a = color.alphaF();
  return c;
}

Image::Color Image::getAveragedSlice(unsigned int x) const {
  Color avgPix;
  int numSamples = 0;
  for(int y = 0; y < m_qObject.height()/4; ++y) {
  //for(int y = 0; y < 1; ++y) {
    Color curPix = getPixel(x, y);
    avgPix.r+=curPix.r;
    avgPix.g+=curPix.g;
    avgPix.b+=curPix.b;
    avgPix.a+=curPix.a;
    numSamples++;
  }
  avgPix.r/=numSamples;
  avgPix.g/=numSamples;
  avgPix.b/=numSamples;
  avgPix.a/=numSamples;
  return avgPix;
}

bool Image::containTransparentPixels() const
{
  for(int y = 0; y < m_qObject.height(); ++y) {
    for(int x = 0; x < m_qObject.width(); ++x) {
      QColor color = m_qObject.pixelColor(x, y);
      if(color.alphaF() < 1.0f) {
        return true;
      }
    }
  }
  return false;
}
}
