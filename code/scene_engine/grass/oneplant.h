#pragma once
#include <glm/vec3.hpp>
#include <renderer_engine/drawdata.h>
#include "../renderpass.h"

namespace sa {
class RenderContext;
class ConfigurationManager;
class RenderDevice;

class OnePlant
{
public:
  /**
   * @brief The OnePlant class represents one plant of grass.
   * Its rendered as a cross, that is three rectangles as seen below.
   *
   *     \  /
   *      \/
   *    ------
   *      /\
   *     /  \
   */
  struct Vertex {
    float x, y, z, u, v;
  };
  struct Face {
    unsigned int v0, v1, v2;
  };

  float HalfSizeOfPlant = 0.5f;


  OnePlant(float halfSizeOfPlant, const glm::vec3& position);

  void toGPU(const sa::ConfigurationManager &config, sa::RenderDevice *device, sa::RenderContext *context);
  void update(float /*dt*/);
  void setPosition(const glm::vec3 &position);
  DrawDataList getDrawData(RenderPass pass);

private:
  glm::vec3 m_position;
  DrawData m_drawData;

};


}
