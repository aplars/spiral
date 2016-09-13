#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <math/Vector4T.h>
#include <glm/vec3.hpp>
namespace sa {
class DirectionalLight
{
public:
  ~DirectionalLight();
  DirectionalLight(
      const glm::vec3& direction,
      const Vector4T<float>& diffuse,
      const Vector4T<float>& ambient);

  const glm::vec3& direction() const;
  void setDirection(const glm::vec3 &direction);


  const Vector4T<float>& diffuse() const;
  void setDiffuse(const Vector4T<float> &diffuse);

  const Vector4T<float>& ambient() const;
  void setAmbient(const Vector4T<float> &ambient);

private:
  glm::vec3 m_direction;
  Vector4T<float> m_diffuse;
  Vector4T<float> m_ambient;

};
}
#endif // DIRECTIONALLIGHT_H
