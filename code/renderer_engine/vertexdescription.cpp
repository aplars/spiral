#include "vertexdescription.h"

namespace sa {
VertexDescriptionElement::VertexDescriptionElement(int shaderAttribLocation, Type type, int numberOfComponents)
  : m_shaderAttribLocation(shaderAttribLocation)
  , m_type(type)
  , m_numberOfComponents(numberOfComponents) { }

int VertexDescriptionElement::shaderAttribLocation() const
{
  return m_shaderAttribLocation;
}

void VertexDescriptionElement::setShaderAttribLocation(int shaderAttribLocation)
{
  m_shaderAttribLocation = shaderAttribLocation;
}

VertexDescriptionElement::Type VertexDescriptionElement::type() const
{
  return m_type;
}

void VertexDescriptionElement::setType(const Type &type)
{
  m_type = type;
}

int VertexDescriptionElement::numberOfComponents() const
{
  return m_numberOfComponents;
}

void VertexDescriptionElement::setNumberOfComponents(int numberOfComponents)
{
  m_numberOfComponents = numberOfComponents;
}

VertexDescription::VertexDescription(std::initializer_list<VertexDescriptionElement> elements) {
  m_elements.resize(elements.size());
  std::copy(elements.begin(), elements.end(), m_elements.begin());
}

int VertexDescription::sizeInBytes() const {
  return std::accumulate(m_elements.begin(), m_elements.end(), 0, [](int sum, const VertexDescriptionElement& elem) {
    return sum+elem.sizeInBytes();
  });
}
}
