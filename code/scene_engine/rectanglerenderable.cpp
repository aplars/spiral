#include "rectanglerenderable.h"
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/rendercontext.h>

#include <renderer_engine/vertexbuffer.h>
#include <renderer_engine/vertexdescription.h>
#include <config/config.h>
#include <glm/gtc/matrix_transform.hpp>
#include "math/mat4ext.h"

namespace sa {
RectangleRenderable::~RectangleRenderable()
{

}

RectangleRenderable::RectangleRenderable(float posx, float posy, float posz, float hw, float hh, std::string vertexShader, std::string fragmentShader)
  : m_position(posx, posy, posz)
  , m_size(hw, hh)
  , m_vertexShader(vertexShader)
  , m_fragmentShader(fragmentShader)
{

}

void RectangleRenderable::setPosition(const glm::vec3 &pos) {
  m_position = pos;
}

void RectangleRenderable::toGPU(const ConfigurationManager& config, RenderDevice *device, RenderContext *context)
{
  std::string dataDir = config.getParam("DATA_DIR");

  std::set<std::string> defines;
  if(m_vertexShader.empty() && m_fragmentShader.empty()) {
    m_shaderProgram = device->createShaderProgramFromFile(
          (dataDir + "/shaders/debugshader.vsh").c_str(),
          (dataDir + "/shaders/debugshader.fsh").c_str(),
          defines);
  }
  else {
    m_shaderProgram = device->createShaderProgramFromFile(
          (m_vertexShader).c_str(),
          (m_fragmentShader).c_str(),
          defines);
  }
  float vertices[] {
    // front
    -m_size[0], -m_size[1],  0, 0, 0,
     m_size[0], -m_size[1],  0, 1, 0,
     m_size[0],  m_size[1],  0, 1, 1,
    -m_size[0],  m_size[1],  0, 0, 1
  };

  unsigned int faces[] = {
    // front
    0, 1, 2,
    2, 3, 0,
  };
  VertexBufferPtr vb = device->createVertexBuffer(vertices, 5*4*sizeof(float));

  int posAttr = m_shaderProgram->attributeLocation("posAttr");
  int texAttr = m_shaderProgram->attributeLocation("texAttr");
  VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
  };

  VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);

  IndexBufferPtr ib = device->createIndexBuffer(faces, 2*3);

  m_drawData.BlendingFunction = Blending::Normal;
  m_drawData.IsTwoSided = false;
  m_drawData.Current_SP = m_shaderProgram;
  m_drawData.VAO = vao;
  m_drawData.IB = ib;
  glm::mat4 modelMatrix;
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = glm::translate(modelMatrix, m_position);
  m_drawData.Uniforms.Vec4Uniforms["u_color"] = glm::vec4(1,0,0,0.5);
  context->resetCurrentState();
}

void RectangleRenderable::update(float dt) {
  glm::mat4 modelMatrix;
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = glm::translate(modelMatrix, m_position);
}

DrawData RectangleRenderable::getDrawData()
{
  return m_drawData;
}
}
