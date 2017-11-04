#pragma once

#include <vector>
#include <array>
#include <renderer_engine/drawdata.h>
#include <glm/vec3.hpp>

namespace sa {
class RenderDevice;
class RenderContext;
class Config;

class CubeRenderable
{
public:
  struct Vertex {
    Vertex(float xx, float yy, float zz)
      : x(xx)
      , y(yy)
      , z(zz) {}

    Vertex(std::array<float, 3> elements) {
      int pos = 0;
      for(float e : elements) {
        if(pos == 0)
          x = e;
        if(pos == 1)
          y = e;
        if(pos == 2)
          z = e;
        ++pos;
      }
    }

    float x;
    float y;
    float z;
  };

  struct Face {
    Face(unsigned int aa, unsigned int bb, unsigned int cc)
      : a(aa)
      , b(bb)
      , c(cc)
    { }

    Face(std::array<unsigned int, 3> elements) {
      int pos = 0;
      for(float e : elements) {
        if(pos == 0)
          a = e;
        if(pos == 1)
          b = e;
        if(pos == 2)
          c = e;
        ++pos;
      }
    }

    unsigned int a;
    unsigned int b;
    unsigned int c;
  };

  ~CubeRenderable();
  CubeRenderable(float posx, float posy, float posz, float hw, float hh, float hd);
  void setPosition(const glm::vec3& pos);
  void toGPU(const Config& config, RenderDevice* device, RenderContext* context);
  void update(float dt);
  const DrawData& getDrawData() const;


private:
  glm::vec3 m_position;
  glm::vec3 m_size;
  DrawData m_drawData;

};
typedef std::shared_ptr<CubeRenderable> CubeRenderablePtr;
}
