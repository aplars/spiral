#include "indexbuffer.h"

namespace sa {

IndexBuffer::~IndexBuffer()
{
  m_glBuffer.destroy();
}

IndexBuffer::IndexBuffer(int numberOfIndices)
  : m_numberOfIndices(numberOfIndices)
  , m_sizeInBytes(sizeof(int)*numberOfIndices)
  , m_glBuffer(QOpenGLBuffer::IndexBuffer)
{
  m_glBuffer.create();
}

void IndexBuffer::write(const unsigned int *data)
{
  m_glBuffer.bind();
  m_glBuffer.allocate(data, m_sizeInBytes);
}

void IndexBuffer::bind()
{
  m_glBuffer.bind();
}

void IndexBuffer::release()
{
  m_glBuffer.release();
}

void IndexBuffer::releaseAll() {
  QOpenGLBuffer::release(QOpenGLBuffer::Type::IndexBuffer);
}

unsigned int IndexBuffer::numberOfIndices() const
{
  return m_numberOfIndices;
}
}
