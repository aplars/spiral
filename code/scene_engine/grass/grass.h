#pragma once
#include <glm/vec2.hpp>
#include <renderer_engine/drawdata.h>
#include "oneplant.h"

namespace sa {
class Config;
class RenderDevice;
class RenderContext;

class Grass
{
public:
  Grass(const glm::vec2& extentsMin, const glm::vec2& extentsMax, float minSpacing, float maxSpacing,float groundPlane);

  void toGPU(const Config& config, unsigned int numberOfShadowCascades, RenderDevice* device, RenderContext* context);

  void update(float dt);

  DrawDataList getDrawData(RenderPass pass);
private:
  glm::vec2 m_extentsMin;
  glm::vec2 m_extentsMax;
  float m_groundPlane;
  OnePlant m_onePlant;
};

}
