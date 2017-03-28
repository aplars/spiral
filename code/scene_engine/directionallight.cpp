#include "directionallight.h"
#include <glm/geometric.hpp>

namespace sa {
DirectionalLight::~DirectionalLight()
{ }

DirectionalLight::DirectionalLight(const glm::vec3& direction,
                                   const glm::vec4 &diffuse,
                                   const glm::vec4 &ambient)
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

const glm::vec4 &DirectionalLight::diffuse() const
{
  return m_diffuse;
}

void DirectionalLight::setDiffuse(const glm::vec4 &diffuse)
{
  m_diffuse = diffuse;
}

const glm::vec4 &DirectionalLight::ambient() const
{
  return m_ambient;
}

void DirectionalLight::setAmbient(const glm::vec4 &ambient)
{
  m_ambient = ambient;
}
}
