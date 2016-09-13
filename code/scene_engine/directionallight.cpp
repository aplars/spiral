#include "directionallight.h"
#include <glm/geometric.hpp>

namespace sa {
DirectionalLight::~DirectionalLight()
{ }

DirectionalLight::DirectionalLight(const glm::vec3& direction,
                                   const Vector4T<float>& diffuse,
                                   const Vector4T<float>& ambient)
  : m_direction(glm::normalize(direction))
  , m_diffuse(diffuse)
  , m_ambient(ambient)
{ }

const glm::vec3& DirectionalLight::direction() const
{
  return m_direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction)
{
  m_direction = direction;
}

const Vector4T<float>& DirectionalLight::diffuse() const
{
  return m_diffuse;
}

void DirectionalLight::setDiffuse(const Vector4T<float> &diffuse)
{
  m_diffuse = diffuse;
}

const Vector4T<float>& DirectionalLight::ambient() const
{
  return m_ambient;
}

void DirectionalLight::setAmbient(const Vector4T<float> &ambient)
{
  m_ambient = ambient;
}
}
