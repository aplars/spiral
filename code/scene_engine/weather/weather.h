#pragma once

#include <glm/vec3.hpp>

namespace sa {
class Weather
{
public:
  Weather();

  float windStrenght() const;
  void setWindStrenght(float windStrenght);

  const glm::vec3& windDirection() const;
  void setWindDirection(const glm::vec3 &windDirection);

private:
  float m_windStrenght = 0.0f;
  glm::vec3 m_windDirection;
};
}
