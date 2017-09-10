#include "weather.h"
namespace sa {
Weather::Weather()
{

}

float Weather::windStrenght() const
{
  return m_windStrenght;
}

void Weather::setWindStrenght(float windStrenght)
{
  m_windStrenght = windStrenght;
}

const glm::vec3& Weather::windDirection() const
{
  return m_windDirection;
}

void Weather::setWindDirection(const glm::vec3 &windDirection)
{
  m_windDirection = windDirection;
}
}

