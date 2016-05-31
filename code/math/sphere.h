#ifndef SPHERE_H
#define SPHERE_H
#include "Vector3T.h"
#include <array>
#include <limits>
#include "ScalarT.h"

namespace sa
{
template <typename T>
class Sphere
{
public:
  Sphere() {

  }

  Sphere(const Vector3T<T>& position, T radius)
    : m_positon(position)
    , m_radius(radius)
  { }


  template <std::size_t N>
  static Sphere<T> createFromPoints(std::array<Vector3T<T>, N>& points) {
    Vector3T<T> min(
          std::numeric_limits<T>::max(),
          std::numeric_limits<T>::max(),
          std::numeric_limits<T>::max());
    Vector3T<T> max(
          -std::numeric_limits<T>::max(),
          -std::numeric_limits<T>::max(),
          -std::numeric_limits<T>::max());

    for(const Vector3T<T>& point : points) {
      for(int i = 0; i < 3; ++i) {
        max[i] = Max(max[i], point[i]);
        min[i] = Min(min[i], point[i]);
      }
    }
    Vector3T<T> mm = (max-min);
    Vector3T<T> centrum = mm*0.5 + min;
    return Sphere<T>(centrum, (max-centrum).GetNorm());
  }

  const Vector3T<T>& getPosition() const { return m_positon; }

  T getRadius() const { return m_radius; }

private:
  Vector3T<T> m_positon;
  T m_radius = 0;

};

}
#endif // SPHERE_H
