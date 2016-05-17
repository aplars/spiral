#include "debugentity.h"
#include "cuberenderable.h"

namespace sa {

DebugEntityBox::~DebugEntityBox()
{

}

DebugEntityBox::DebugEntityBox(float posx, float posy, float posz, float hw, float hh, float hd)
  :   m_renderable(new CubeRenderable(posx, posy, posz, hw, hh, hd))
{

}

void DebugEntityBox::toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context) {
  m_renderable->toGPU(config, device, context);
}

const DrawData& DebugEntityBox::getDrawData() const {
  return m_renderable->getDrawData();
}

}