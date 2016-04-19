#pragma once
#include <deque>
#include <QOpenGLFunctions>

namespace sa {


class VertexDescriptionElement {
public:
  enum class Type {
    INT = GL_INT,
    FLOAT = GL_FLOAT
  };

  static int sizeofTypeInBytes(Type type)
  {
    switch(type) {
    case Type::INT:
      return sizeof(int);
    case Type::FLOAT:
      return sizeof(float);
    }
    return 0;
  }

  VertexDescriptionElement() {}
  VertexDescriptionElement(int shaderAttribLocation, Type type, int numberOfComponents);
  int shaderAttribLocation() const;
  void setShaderAttribLocation(int shaderAttribLocation);

  Type type() const;
  void setType(const Type &type);

  int numberOfComponents() const;
  void setNumberOfComponents(int numberOfComponents);

  int sizeInBytes() const { return sizeofTypeInBytes(m_type) * m_numberOfComponents; }
private:
  int m_shaderAttribLocation = 0;
  Type m_type = Type::INT;
  int m_numberOfComponents = 0;
};

class VertexDescription {
public:
  typedef std::deque<VertexDescriptionElement> VertexDescriptionElements;
  VertexDescription(std::initializer_list<VertexDescriptionElement> elements);
  VertexDescriptionElements::const_iterator begin() { return m_elements.begin(); }
  VertexDescriptionElements::const_iterator end() { return m_elements.end(); }

  int sizeInBytes() const;
private:
  VertexDescriptionElements m_elements;
};
}
