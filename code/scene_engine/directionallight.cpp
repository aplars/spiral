#include "directionallight.h"
namespace sa {
DirectionalLight::~DirectionalLight()
{ }

DirectionalLight::DirectionalLight(const Vector3T<float>& direction,
                                   const Vector4T<float>& ambient,
                                   const Vector4T<float>& diffuse)
  : m_direction(direction.GetNormalized())
  , m_ambient(ambient)
  , m_diffuse(diffuse)
{ }

const Vector3T<float>& DirectionalLight::direction() const
{
  return m_direction;
}

void DirectionalLight::setDirection(const Vector3T<float> &direction)
{
  m_direction = direction;
}

const Vector4T<float>& DirectionalLight::ambient() const
{
  return m_ambient;
}

void DirectionalLight::setAmbient(const Vector4T<float> &ambient)
{
  m_ambient = ambient;
}

const Vector4T<float>& DirectionalLight::diffuse() const
{
  return m_diffuse;
}

void DirectionalLight::setDiffuse(const Vector4T<float> &diffuse)
{
  m_diffuse = diffuse;
}
}
