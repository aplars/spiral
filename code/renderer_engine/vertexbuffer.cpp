#include "vertexbuffer.h"
namespace sa {
VertexBuffer::~VertexBuffer()
{
  m_glBuffer.destroy();
}

VertexBuffer::VertexBuffer(int sizeInBytes)
  : m_sizeInBytes(sizeInBytes)
  , m_glBuffer(QOpenGLBuffer::VertexBuffer)
{
  m_glBuffer.create();
}

void VertexBuffer::write(const void* data) {
  m_glBuffer.bind();
  m_glBuffer.allocate(data, m_sizeInBytes);
}

void VertexBuffer::bind() {
  m_glBuffer.bind();
}

void VertexBuffer::release() {
  m_glBuffer.release();
}

void* VertexBuffer::map() {
  return m_glBuffer.map(QOpenGLBuffer::Access::ReadWrite);
}

void VertexBuffer::unmap() {
  m_glBuffer.unmap();
}

}
