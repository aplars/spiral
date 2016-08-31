#include "lightshafts.h"
#include <renderer_engine/rendercontext.h>
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/texture.h>
#include <config/config.h>
namespace sa {
Matrix44T<float> orthoProjection;

LightShafts::LightShafts(const ConfigurationManager& config)
  : m_cubeRenderable(0, 0, 0, 50, 50, 100, (config.getParam("DATA_DIR") + "/shaders/lightshafts.vsh").c_str(), (config.getParam("DATA_DIR") + "/shaders/lightshafts.fsh").c_str())
{
  //orthoProjection = sa::Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0f), 1, 0.1, 500);
  orthoProjection = sa::Matrix44T<float>::GetOrthographicProjection(-100, 100, -100, 100, 0.0, 50);
}

void LightShafts::toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context) {
  m_cubeRenderable.toGPU(config, device, context);  
}

DrawData LightShafts::getDrawData() {
  DrawData drawData = m_cubeRenderable.getDrawData();

  drawData.Uniforms.Matrix4Uniforms["u_projectionMatrix"] = orthoProjection;
  drawData.Uniforms.Matrix4Uniforms["u_viewMatrix"] = Matrix44T<float>::GetIdentity();
  return drawData;
}
}
