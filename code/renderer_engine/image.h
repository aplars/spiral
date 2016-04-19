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
  ~Image();
  Image() {}
  Image(const unsigned char* data, int w, int h);
  Image(const char* fileName);
  Image(const std::string &fileName) : Image(fileName.c_str()) {}

  const QImage& getQObject() const { return m_qObject; }
  bool isNull() const { return m_isNull; }
private:
  QImage m_qObject;
  bool m_isNull = true;
};
}
#endif // IMAGE_H
