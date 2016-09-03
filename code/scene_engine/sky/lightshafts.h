#ifndef LIGHTSHAFTS_H
#define LIGHTSHAFTS_H
#include <renderer_engine/drawdata.h>
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
  const Matrix44T<float>& getProjection() const;
  DrawData getDrawData();
private:
  Matrix44T<float> m_projection;
  RectangleRenderable m_rectangleRenderable;
};
}
#endif // LIGHTSHAFTS_H
