#pragma once
#include <renderer_engine/drawdata.h>
#include <glm/mat4x4.hpp>
#include "../rectanglerenderable.h"

namespace sa {
class ConfigurationManager;
class RenderDevice;
class RenderContext;

class LightShafts
{
public:

  LightShafts(const ConfigurationManager &config);
  void toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context);
  const glm::mat4 &getProjection() const;
  DrawData getDrawData();
  void setExposure(float exposure);
  void setDecay(float decay);
  void setDensity(float density);
  void setWeight(float weight);

private:
  glm::mat4 m_projection;
  RectangleRenderable m_rectangleRenderable;
  float m_exposure = 0.0034f;
  float m_decay = 1.0f;
  float m_density = 0.84f;
  float m_weight = 5.65f;
};
}
