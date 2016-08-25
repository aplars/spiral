#include "cuberenderable.h"
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/rendercontext.h>

#include <renderer_engine/vertexbuffer.h>
#include <renderer_engine/vertexdescription.h>
#include <config/config.h>

namespace sa {
CubeRenderable::~CubeRenderable()
{

}

CubeRenderable::CubeRenderable(float posx, float posy, float posz, float hw, float hh, float hd)
  : m_position(posx, posy, posz)
  , m_size(hw, hh, hd)
{

}

void CubeRenderable::setPosition(const Vector3T<float>& pos) {
  m_position = pos;
}

void CubeRenderable::toGPU(const ConfigurationManager& config, RenderDevice *device, RenderContext *context)
{
  float vertices[] {
    // front
    -m_size[0], -m_size[1],  m_size[2],
     m_size[0], -m_size[1],  m_size[2],
     m_size[0],  m_size[1],  m_size[2],
    -m_size[0],  m_size[1],  m_size[2],
    // back
    -m_size[0], -m_size[1], -m_size[2],
     m_size[0], -m_size[1], -m_size[2],
     m_size[0],  m_size[1], -m_size[2],
    -m_size[0],  m_size[1], -m_size[2],
  };

  unsigned int faces[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    3, 2, 6,
    6, 7, 3,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // left
    4, 0, 3,
    3, 7, 4,
    // right
    1, 5, 6,
    6, 2, 1,
  };
  VertexBufferPtr vb = device->createVertexBuffer(vertices, 3*12*sizeof(float));

  std::string dataDir = config.getParam("DATA_DIR");

  std::set<std::string> defines;
  ShaderProgramPtr sp = device->createShaderProgramFromFile(
        (dataDir + "/shaders/debugshader.vsh").c_str(),
        (dataDir + "/shaders/debugshader.fsh").c_str(),
        defines);
  int posAttr = sp->attributeLocation("posAttr");
  VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
  };

  VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);

  IndexBufferPtr ib = device->createIndexBuffer(faces, 12*3);

  m_drawData.BlendingFunction = Blending::Normal;
  m_drawData.IsTwoSided = false;
  m_drawData.SP = sp;
  m_drawData.VAO = vao;
  m_drawData.IB = ib;
  //m_drawData.Matrix4Uniforms[sp->uniformLocation("u_modelMatrix")] = Matrix44T<float>::GetIdentity();
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = Matrix44T<float>::GetTranslate(m_position);
  m_drawData.Uniforms.Vec4Uniforms["u_color"] = Vector4T<float>(1,0,0,0.5);
  context->resetCurrentState();
}

void CubeRenderable::update(float dt) {
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = Matrix44T<float>::GetTranslate(m_position);
}

const DrawData& CubeRenderable::getDrawData() const
{
  return m_drawData;
}
}
