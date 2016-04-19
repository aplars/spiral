#pragma once
#include <QOpenGLBuffer>
#include <memory>

namespace sa {
class IndexBuffer {
public:
  ~IndexBuffer();
  IndexBuffer(int numberOfIndices);
  void write(const unsigned int* data);
  void bind();
  void release();
  static void releaseAll();
  unsigned int numberOfIndices() const;

private:
  int m_numberOfIndices;
  int m_sizeInBytes;
  QOpenGLBuffer m_glBuffer;
};
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
}
