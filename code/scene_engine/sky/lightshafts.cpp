#include "lightshafts.h"
#include <renderer_engine/rendercontext.h>
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/texture.h>
#include <config/config.h>
#include <glm/gtc/matrix_transform.hpp>

namespace sa {

LightShafts::LightShafts(const Config& config)
  : m_rectangleRenderable(0, 0, 0, 100, 100, (config.getParam("DATA_DIR") + "/shaders/lightshafts.vsh").c_str(), (config.getParam("DATA_DIR") + "/shaders/lightshafts.fsh").c_str())
{
  m_projection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.0f, 0.01f);
}

void LightShafts::toGPU(const Config& config, RenderDevice* device, RenderContext* context) {
  m_rectangleRenderable.toGPU(config, device, context);
}

const glm::mat4 &LightShafts::getProjection() const
{
  return m_projection;
}

DrawData LightShafts::getDrawData() {
  DrawData drawData = m_rectangleRenderable.getDrawData();
  /*
    u_uniformExposure = 0.0034f;
    u_uniformDecay = 1.0f;
    u_uniformDensity = 0.84f;
    u_uniformWeight = 5.65f;

  */
  drawData.BlendingFunction = Blending::SRC_ALPHA_ONE;
  drawData.Uniforms.Matrix4Uniforms["u_projectionMatrix"] = m_projection;
  drawData.Uniforms.Matrix4Uniforms["u_viewMatrix"] = glm::mat4(1.0f);
  drawData.Uniforms.FloatUniforms["u_exposure"] = m_exposure;
  drawData.Uniforms.FloatUniforms["u_decay"] = m_decay;
  drawData.Uniforms.FloatUniforms["u_density"] = m_density;
  drawData.Uniforms.FloatUniforms["u_weight"] = m_weight;
  return drawData;
}

void LightShafts::setExposure(float exposure)
{
  m_exposure = exposure;
}

void LightShafts::setDecay(float decay)
{
  m_decay = decay;
}

void LightShafts::setDensity(float density)
{
  m_density = density;
}

void LightShafts::setWeight(float weight)
{
  m_weight = weight;
}
}
