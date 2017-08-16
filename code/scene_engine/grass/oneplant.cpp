#include "oneplant.h"
#include <config/config.h>
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/vertexdescription.h>
#include <renderer_engine/rendercontext.h>
#include <glm/gtc/matrix_transform.hpp>

namespace sa {

OnePlant::OnePlant(float halfSizeOfPlant, const glm::vec3 &position)
  : HalfSizeOfPlant(halfSizeOfPlant)
  , m_position(position)
{
}

void OnePlant::toGPU(const sa::ConfigurationManager &config, sa::RenderDevice *device, sa::RenderContext *context)
{
  ShaderProgramPtr sp = device->createShaderProgramFromFile(config.getParam("DATA_DIR") + "/shaders/grassshader.vsh", config.getParam("DATA_DIR") + "/shaders/grassshader.fsh");

  TexturePtr tex = device->createTextureFromFile((config.getParam("DATA_DIR") + "/textures/grasspack.png").c_str(), Texture::WrapMode::ClampToEdge);

  std::string dataDir = config.getParam("DATA_DIR");

  float vertices[] {
        -HalfSizeOfPlant, 0,  HalfSizeOfPlant, 0, 0,
        HalfSizeOfPlant,  0,  -HalfSizeOfPlant, 0.25f, 0,
        HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  -HalfSizeOfPlant, 0.25f, 1,
        -HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  HalfSizeOfPlant, 0, 1,
        //
        -HalfSizeOfPlant, 0,  -HalfSizeOfPlant, 0, 0,
        HalfSizeOfPlant, 0,   HalfSizeOfPlant, 0.25f, 0,
        HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,   HalfSizeOfPlant, 0.25f, 1,
        -HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  -HalfSizeOfPlant, 0, 1,

        //
        -HalfSizeOfPlant, 0,  -0, 0, 0,
        HalfSizeOfPlant, 0,   -0, 0.25f, 0,
        HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,   -0, 0.25f, 1,
        -HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  -0, 0, 1
  };

  unsigned int faces[] = {
    //
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8
  };
  VertexBufferPtr vb = device->createVertexBuffer(vertices, 5*12*sizeof(float));

  int posAttr = sp->attributeLocation("posAttr");
  int texAttr = sp->attributeLocation("texAttr");
  VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
  };

  VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);

  IndexBufferPtr ib = device->createIndexBuffer(faces, 18);



  m_drawData.IsTwoSided = true;
  m_drawData.BlendingFunction = Blending::None;
  m_drawData.TEX[0] = tex;
  m_drawData.Current_SP = sp;
  m_drawData.VAO = vao;
  m_drawData.IB = ib;

  m_drawData.AlphaFunction = Alpha::Greater;
  m_drawData.AlphaValue = 0.1f;

  glm::mat4 modelMatrix;
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = glm::translate(modelMatrix, m_position);
  //m_drawData.Uniforms.Vec4Uniforms["u_color"] = glm::vec4(1,0,0,0.5);
  context->resetCurrentState();
}

void OnePlant::update(float /*dt*/) {
  glm::mat4 modelMatrix;
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = glm::translate(modelMatrix, m_position);
}

void OnePlant::setPosition(const glm::vec3 &position)
{
  m_position = position;
}

DrawDataList OnePlant::getDrawData(RenderPass pass)
{
  DrawDataList ddlist;
  if(pass == RenderPass::Uber) {
    ddlist.push_back(m_drawData);
  }
  return ddlist;
}
}
