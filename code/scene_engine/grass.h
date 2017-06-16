#pragma once
#include <glm/vec3.hpp>
#include <renderer_engine/drawdata.h>

namespace sa {
class ConfigurationManager;
class RenderDevice;
class RenderContext;

class Grass
{
public:
  Grass(const glm::vec3& extentsMin, const glm::vec3& extentsMax);

  void toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context);

  //void update(float dt, const glm::vec3& cameraPosition);
private:
  glm::vec3 m_extentsMin;
  glm::vec3 m_extentsMax;
  DrawData m_drawData;
};

}
