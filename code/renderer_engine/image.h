#ifndef IMAGE_H
#define IMAGE_H
#include <QImage>

namespace sa {
/**
 * @brief The Image class represents an uncompressed image in the 32 bit argb format.
 */
class Image
{
public:
  struct Color {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
  };

  ~Image();
  Image() {}
  Image(const unsigned char* data, int w, int h);
  Image(const char* fileName);
  Image(const std::string &fileName) : Image(fileName.c_str()) {}
  void save(const std::string &fileName);

  unsigned int getWidth() const;
  unsigned int getHeight() const;


  Color getPixel(unsigned int x, unsigned y) const;
  Color getAveragedSlice(unsigned int x) const;

  const QImage& getQObject() const { return m_qObject; }
  bool isNull() const { return m_isNull; }

  bool containTransparentPixels() const;
private:
  QImage m_qObject;
  bool m_isNull = true;
};
}
#endif // IMAGE_H
