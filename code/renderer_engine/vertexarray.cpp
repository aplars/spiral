#include "vertexarray.h"
namespace sa {
VertexArray::~VertexArray() {
  m_glObject.destroy();
}

VertexArray::VertexArray() {
  m_glObject.create();
}

void VertexArray::bind() {
  m_glObject.bind();
}

void VertexArray::release() {
  m_glObject.release();
}
}
