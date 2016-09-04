#ifndef RECTANGLERENDERABLE_H
#define RECTANGLERENDERABLE_H
#include <vector>
#include <array>
#include <renderer_engine/drawdata.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace sa {
class RenderDevice;
class RenderContext;
class ConfigurationManager;

class RectangleRenderable
{
public:
  struct Vertex {
    Vertex(float xx, float yy, float zz, float uu, float vv)
      : x(xx)
      , y(yy)
      , z(zz)
      , u(uu)
      , v(vv) {}

    Vertex(std::array<float, 5> elements) {
      int pos = 0;
      for(float e : elements) {
        if(pos == 0)
          x = e;
        if(pos == 1)
          y = e;
        if(pos == 2)
          z = e;
        if(pos == 3)
          u = e;
        if(pos == 4)
          v = e;
        ++pos;
      }
    }

    float x;
    float y;
    float z;
    float u;
    float v;
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

  ~RectangleRenderable();
  RectangleRenderable(float posx, float posy, float posz, float hw, float hh, std::string vertexShader="", std::string fragmentShader="");
  void setPosition(const glm::vec3& pos);
  void toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context);
  void update(float dt);
  DrawData getDrawData();


private:
  std::string m_vertexShader;
  std::string m_fragmentShader;

  ShaderProgramPtr m_shaderProgram;
  glm::vec3 m_position;
  glm::vec2 m_size;
  DrawData m_drawData;
};
}
#endif // RECTANGLERENDERABLE_H
