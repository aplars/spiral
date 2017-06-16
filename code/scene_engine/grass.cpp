#include "grass.h"
#include "config/config.h"
#include "renderer_engine/renderdevice.h"
#include "renderer_engine/rendercontext.h"

namespace sa {

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
class OnePlant {
  struct Vertex {
    float x, y, z, u, v;
  };
  struct Face {
    unsigned int v0, v1, v2;
  };

  float HalfSizeOfPlant = 0.5f;

  DrawData m_drawData;

  void toGPU(const sa::ConfigurationManager &config, sa::RenderDevice *device, sa::RenderContext *context)
  {
    auto sp = device->createShaderProgramFromFile("vp", "fp", "gp");

    device->createTextureFromFile(config.getParam("grass.dds").c_str(), Texture::WrapMode::ClampToEdge);

    std::vector<Face> faces;
    std::vector<Vertex> vertices;
    //Create the faces
    vertices.push_back({-HalfSizeOfPlant, -HalfSizeOfPlant, 0.0f, 0.0f, 0.0f});
    vertices.push_back({-HalfSizeOfPlant,  HalfSizeOfPlant, 0.0f, 0.0f, 1.0f});
    vertices.push_back({ HalfSizeOfPlant,  HalfSizeOfPlant, 0.0f, 1.0f, 1.0f});
    vertices.push_back({ HalfSizeOfPlant, -HalfSizeOfPlant, 0.0f, 1.0f, 0.0f});

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

    m_drawData.VAO = vertexArray;
    m_drawData.IB = indexBuffer;
  }
};


Grass::Grass(const glm::vec3& extentsMin, const glm::vec3& extentsMax)
  : m_extentsMin(extentsMin)
  , m_extentsMax(extentsMax)
{ }

void sa::Grass::toGPU(const sa::ConfigurationManager &config, sa::RenderDevice *device, sa::RenderContext *context)
{

}
}
