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

  DrawData getDrawData();
private:
  RectangleRenderable m_cubeRenderable;
};
}
#endif // LIGHTSHAFTS_H
