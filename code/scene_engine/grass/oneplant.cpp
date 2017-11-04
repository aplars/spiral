#include "oneplant.h"
#include <config/config.h>
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/vertexdescription.h>
#include <renderer_engine/rendercontext.h>
#include <glm/gtc/matrix_transform.hpp>

namespace sa {

OnePlant::OnePlant(float halfSizeOfPlant, const std::vector<glm::vec3> &positions)
  : HalfSizeOfPlant(halfSizeOfPlant)
  , m_positions(positions)
{
}

void OnePlant::toGPU(const sa::Config &config, unsigned int numberOfShadowCascades, sa::RenderDevice *device, sa::RenderContext *context)
{
  std::set<std::string> globalDefines;
  globalDefines.insert("NUMBER_OF_CASCADES " + std::to_string(numberOfShadowCascades));

  ShaderProgramPtr sp = device->createShaderProgramFromFile(config.getParam("DATA_DIR") + "/shaders/grassshader.vsh", config.getParam("DATA_DIR") + "/shaders/grassshader.fsh", globalDefines);
  ShaderProgramPtr SunLightShaftsSp = device->createShaderProgramFromFile(config.getParam("DATA_DIR") + "/shaders/grassshader.vsh", config.getParam("DATA_DIR") + "/shaders/grassblackshader.fsh", globalDefines);
  TexturePtr tex = device->createTextureFromFile((config.getParam("DATA_DIR") + "/textures/grasspack.png").c_str(), Texture::WrapMode::ClampToEdge);

  std::string dataDir = config.getParam("DATA_DIR");

  float vertices[] {
        -HalfSizeOfPlant,  0,                       HalfSizeOfPlant,    0,  1, 0,      0,     0, 0,
        HalfSizeOfPlant,   0,                       -HalfSizeOfPlant,   0,  1, 0,      0.25f, 0, 0,
        HalfSizeOfPlant,   2.0f * HalfSizeOfPlant,  -HalfSizeOfPlant,   0,  1, 0,      0.25f, 1, 1,
        -HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  HalfSizeOfPlant,    0,  1, 0,      0,     1, 1,
        //
        -HalfSizeOfPlant,  0,  -HalfSizeOfPlant,                        0,  1, 0,      0,     0, 0,
        HalfSizeOfPlant,   0,   HalfSizeOfPlant,                        0,  1, 0,      0.25f, 0, 0,
        HalfSizeOfPlant,   2.0f * HalfSizeOfPlant,   HalfSizeOfPlant,   0,  1,  0,     0.25f, 1, 1,
        -HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  -HalfSizeOfPlant,   0,  1,  0,     0,     1, 1,

        //
        -HalfSizeOfPlant,  0,                       -0,                 0,  1, 0,      0,     0, 0,
        HalfSizeOfPlant,   0,                       -0,                 0,  1, 0,      0.25f, 0, 0,
        HalfSizeOfPlant,   2.0f * HalfSizeOfPlant,  -0,                 0,  1,  0,     0.25f, 1, 1,
        -HalfSizeOfPlant,  2.0f * HalfSizeOfPlant,  -0,                 0,  1,  0,     0,     1, 1
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
  VertexBufferPtr vb = device->createVertexBuffer(vertices, 9*12*sizeof(float));

  int posAttr = sp->attributeLocation("posAttr");

  int norAttr = sp->attributeLocation("norAttr");

  int texAttr = sp->attributeLocation("texAttr");

  int windAttr = sp->attributeLocation("windAttr");
  VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {norAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
    {windAttr, sa::VertexDescriptionElement::Type::FLOAT, 1},
  };

  VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);

  IndexBufferPtr ib = device->createIndexBuffer(faces, 18);


  m_drawData.NumberOfInstances = m_positions.size();
  m_drawData.IsTwoSided = true;
  m_drawData.BlendingFunction = Blending::None;
  m_drawData.TEX[0] = tex;
  m_drawData.Current_SP = sp;
  m_drawData.Uber_SP = sp;
  m_drawData.SunLightShafts_SP = SunLightShaftsSp;
  m_drawData.VAO = vao;
  m_drawData.IB = ib;

  m_drawData.AlphaFunction = Alpha::Greater;
  m_drawData.AlphaValue = 0.1f;

  glm::mat4 modelMatrix;
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = modelMatrix;//glm::translate(modelMatrix, m_position);

  m_drawData.Uniforms.Vec3ArrayUniforms["u_positions"] = m_positions;

  //m_drawData.Uniforms.Vec4Uniforms["u_color"] = glm::vec4(1,0,0,0.5);
  context->resetCurrentState();
}

void OnePlant::update(float /*dt*/) {
  glm::mat4 modelMatrix;
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = modelMatrix;//glm::translate(modelMatrix, m_position);
  m_drawData.Uniforms.Vec3ArrayUniforms["u_positions"] = m_positions;
}


DrawDataList OnePlant::getDrawData(RenderPass pass)
{
  DrawDataList ddlist;
  if(pass == RenderPass::Uber) {
    m_drawData.Current_SP = m_drawData.Uber_SP;
    ddlist.push_back(m_drawData);
  }
  else if (pass == RenderPass::SunLightShafts) {
    m_drawData.Current_SP = m_drawData.SunLightShafts_SP;
    ddlist.push_back(m_drawData);
  }
  return ddlist;
}
}
