#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <renderer_engine/drawdata.h>
#include "../renderpass.h"

namespace sa {
class ConfigurationManager;
class RenderDevice;
class RenderContext;
namespace sky {
class Sky
{
public:
  struct Vertex {
    Vertex() {}
    Vertex(float xx, float yy, float zz, float uu, float vv)
      : x(xx)
      , y(yy)
      , z(zz)
      , u(uu)
      , v(vv){}


    float x = 0;
    float y = 0;
    float z = 0;
    float u = 0;
    float v = 0;
  };

  Sky();

  Sky(float radius, int Slices, int Sides, float dampening);

  glm::vec3 getSunPosition() const;
  void toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context);

  void update(float dt, const glm::vec3& cameraPosition);

  DrawData getDrawData(RenderPass renderPass);

  double TimeOfDay = 16;
  double JulianDay = 180;
  double SunTheta;
  double SunPhi;
  double Latitude = 44.0;
  double Longitude = 36.0;
  double TimeScale = 1.0;
  float FogDensity = 0.0005;
  bool IsRunningSimulation = false;
private:
  float m_radius = 0;
  std::vector<Vertex> m_vertices;
  std::vector<unsigned int> m_indices;
  ShaderProgramPtr m_spBlackAndWhite;
  DrawData m_drawData;
};
}
}
