#pragma once
#include <memory>
#include <QOpenGLVertexArrayObject>

namespace sa {
class VertexArray {
public:
  ~VertexArray();
  VertexArray();
  void bind();
  void release();
private:
  QOpenGLVertexArrayObject m_glObject;
};
typedef std::shared_ptr<VertexArray> VertexArrayPtr;
}
