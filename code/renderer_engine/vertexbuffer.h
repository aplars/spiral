#pragma once
#include <QOpenGLBuffer>
#include <memory>

namespace sa {
class VertexBuffer
{
public:
  ~VertexBuffer();
  VertexBuffer(int sizeInBytes);

  void write(const void* data);
  void bind();
  void release();
  void* map();
  template <typename T>
  T* mapT() { return static_cast<T*>(map()); }
  void unmap();
private:
  VertexBuffer(const VertexBuffer&);
  const VertexBuffer& operator=(const VertexBuffer&);

  int m_sizeInBytes;
  QOpenGLBuffer m_glBuffer;
};
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
}
