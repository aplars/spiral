#include "grass.h"
#include "config/config.h"
#include "renderer_engine/renderdevice.h"
#include "renderer_engine/rendercontext.h"

namespace sa {


Grass::Grass(const glm::vec2 &extentsMin, const glm::vec2 &extentsMax, float groundPlane)
  : m_extentsMin(extentsMin)
  , m_extentsMax(extentsMax)
{ }

void sa::Grass::toGPU(const sa::ConfigurationManager &config, sa::RenderDevice *device, sa::RenderContext *context)
{

}
}
