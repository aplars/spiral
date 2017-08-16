#pragma once
#include <glm/vec2.hpp>
#include <renderer_engine/drawdata.h>

namespace sa {
class ConfigurationManager;
class RenderDevice;
class RenderContext;

class Grass
{
public:
  Grass(const glm::vec2& extentsMin, const glm::vec2& extentsMax, float groundPlane);

  void toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context);

  //void update(float dt, const glm::vec3& cameraPosition);
private:
  glm::vec2 m_extentsMin;
  glm::vec2 m_extentsMax;
  float m_groundPlane;
  DrawData m_drawData;
};

}
