#include "oneplant.h"
#include <config/config.h>
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/vertexdescription.h>
#include <renderer_engine/rendercontext.h>

namespace sa {

void OnePlant::toGPU(const sa::ConfigurationManager &config, sa::RenderDevice *device, sa::RenderContext *context)
{
  ShaderProgramPtr sp = device->createShaderProgramFromFile(config.getParam("DTATA_DIR") + "/shaders/grass.vp", config.getParam("DTATA_DIR") + "/shaders/grass.fp");

  TexturePtr tex = device->createTextureFromFile((config.getParam("DTATA_DIR") + "/textures/grass.png").c_str(), Texture::WrapMode::ClampToEdge);

  std::vector<Face> faces;
  std::vector<Vertex> vertices;
  //Create the faces
  vertices.push_back({-HalfSizeOfPlant, -HalfSizeOfPlant, -HalfSizeOfPlant, 0.0f, 0.0f});
  vertices.push_back({-HalfSizeOfPlant,  HalfSizeOfPlant, -HalfSizeOfPlant, 0.0f, 1.0f});
  vertices.push_back({ HalfSizeOfPlant,  HalfSizeOfPlant,  HalfSizeOfPlant, 1.0f, 1.0f});
  vertices.push_back({ HalfSizeOfPlant, -HalfSizeOfPlant,  HalfSizeOfPlant, 1.0f, 0.0f});

  faces.push_back({0, 1, 2});
  faces.push_back({0, 1, 3});


  int posAttr = sp->attributeLocation("posAttr");
  int texAttr = sp->attributeLocation("texAttr");
  sa::VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2}
  };


  IndexBufferPtr indexBuffer = device->createIndexBuffer((unsigned int*)faces.data(), faces.size()*3);
  VertexBufferPtr vertexBuffer = device->createVertexBuffer(vertices.data(), sizeof(Vertex)*vertices.size());
  VertexArrayPtr vertexArray = context->createVertexArray(vertexDesc, vertexBuffer);

  m_drawData.Current_SP = sp;
  m_drawData.TEX[0] = tex;
  m_drawData.VAO = vertexArray;
  m_drawData.IB = indexBuffer;
}
}
