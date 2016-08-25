#pragma once
#include <memory>
#include <deque>
#include "renderer_engine/drawdata.h"
namespace sa {
class RenderDevice;
class RenderContext;
class ConfigurationManager;
class CubeRenderable;
typedef std::shared_ptr<CubeRenderable> CubeRenderablePtr;

class DebugEntityBox
{
public:
  ~DebugEntityBox();
  DebugEntityBox(float posx, float posy, float posz, float hw, float hh, float hd);
  void setPosition(const Vector3T<float>& pos);
  void toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context);
  void update(float dt);
  const DrawData& getDrawData() const;

private:
  CubeRenderablePtr m_renderable;
};
}
