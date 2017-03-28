#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sa {
class DirectionalLight
{
public:
  ~DirectionalLight();
  DirectionalLight(
      const glm::vec3& direction,
      const  glm::vec4& diffuse,
      const  glm::vec4& ambient);

  const glm::vec3& direction() const;
  void setDirection(const glm::vec3 &direction);


  const glm::vec4& diffuse() const;
  void setDiffuse(const  glm::vec4 &diffuse);

  const  glm::vec4& ambient() const;
  void setAmbient(const  glm::vec4 &ambient);

private:
  glm::vec3 m_direction;
  glm::vec4 m_diffuse;
  glm::vec4 m_ambient;

};
}
#endif // DIRECTIONALLIGHT_H
