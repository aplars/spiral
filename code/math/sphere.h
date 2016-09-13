#ifndef SPHERE_H
#define SPHERE_H
#include <array>
#include <limits>
#include "ScalarT.h"
#include <glm/vec3.hpp>
namespace sa
{
template <typename T>
class Sphere
{
public:
  Sphere() {

  }

  Sphere(const glm::vec3& position, T radius)
    : m_positon(position)
    , m_radius(radius)
  { }

  template <std::size_t N>
  static Sphere<T> createFromPoints(std::array<glm::vec3, N>& points) {
    glm::vec3 min(
          std::numeric_limits<T>::max(),
          std::numeric_limits<T>::max(),
          std::numeric_limits<T>::max());
    glm::vec3 max(
          -std::numeric_limits<T>::max(),
          -std::numeric_limits<T>::max(),
          -std::numeric_limits<T>::max());

    for(const glm::vec3& point : points) {
      for(int i = 0; i < 3; ++i) {
        max[i] = Max(max[i], point[i]);
        min[i] = Min(min[i], point[i]);
      }
    }
    glm::vec3 mm = (max-min);
    glm::vec3 centrum = 0.5f * mm + min;
    return Sphere<T>(centrum, (max-centrum).length());
  }
  const glm::vec3& getPosition() const { return m_positon; }

  T getRadius() const { return m_radius; }

private:
  glm::vec3 m_positon;
  T m_radius = 0;

};

}
#endif // SPHERE_H
