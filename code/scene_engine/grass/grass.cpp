#include "grass.h"
#include "config/config.h"
#include "renderer_engine/renderdevice.h"
#include "renderer_engine/rendercontext.h"
#include <math/ScalarT.h>

namespace sa {

std::vector<glm::vec3> getPositions(const glm::vec2 &extentsMin, const glm::vec2 &extentsMax, float minSpacing, float maxSpacing) {

  std::vector<glm::vec3> positions;
  float separation = Random(minSpacing, maxSpacing);
  for(float y = extentsMin.y; y < extentsMax.y; y+=separation) {
    separation = Random(minSpacing, maxSpacing);
    for(float x = extentsMin.x; x < extentsMax.x; x+=separation) {
      separation = Random(minSpacing, maxSpacing);
      positions.push_back({x, 0.0f, y});
    }
  }
  return positions;
}

Grass::Grass(const glm::vec2 &extentsMin, const glm::vec2 &extentsMax, float minSpacing, float maxSpacing, float groundPlane)
  : m_extentsMin(extentsMin)
  , m_extentsMax(extentsMax)
  , m_onePlant(10.5f, getPositions(extentsMin, extentsMax, minSpacing, maxSpacing))
{ }

void Grass::toGPU(const sa::Config &config, unsigned int numberOfShadowCascades, sa::RenderDevice *device, sa::RenderContext *context)
{
  m_onePlant.toGPU(config, numberOfShadowCascades, device, context);
}

void Grass::update(float dt)
{
  m_onePlant.update(dt);
}

DrawDataList Grass::getDrawData(RenderPass pass)
{
  return m_onePlant.getDrawData(pass);
}
}
