#ifndef LIGHTSHAFTS_H
#define LIGHTSHAFTS_H
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
private:
  glm::mat4 m_projection;
  RectangleRenderable m_rectangleRenderable;
};
}
#endif // LIGHTSHAFTS_H
