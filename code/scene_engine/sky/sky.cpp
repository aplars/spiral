#include "sky.h"
#include <math/ScalarT.h>
#include <renderer_engine/renderdevice.h>
#include <renderer_engine/rendercontext.h>
#include <renderer_engine/image.h>
#include <config/config.h>

namespace sa {
namespace sky {
Sky::Sky()
  : Sky(1500, 20, 20, 1)
{

}

Sky::Sky(float radius, int slices, int sides, float dampening) {

  m_radius = radius;
  float polyAng = 2.0f * sa::Pi<float>() / sides, ang;

  float vx, vy, vz;

  int i, j;
  for (j = 0; j <= slices; j++)
  {
    ang = j * ( (sa::Pi<float>() / 2) / slices );
    for (i = 0; i <= sides; i++)
    {
      vx = cos(i * polyAng) * cos(ang);
      vy = sin(ang) * dampening;
      vz = sin(i * polyAng) * cos(ang);

      Vertex v;
      v.x = vx * radius;
      v.z = vz * radius;
      v.y = vy * radius;


      v.u = (float)(i) / (float)(sides);
      //v.v = (float)(j) / (float)(slices);
      v.v = (float)(j) / (float)(slices);
      m_vertices.push_back(v);
    }
  }

  for (j = 1; j <= slices; j++)
  {
    for (i = 0; i <= sides; i++)
    {
      m_indices.push_back(j      * (sides + 1) + i);
      m_indices.push_back((j - 1) * (sides + 1) + i);
    }
  }

}

//glm::vec3 makeDirection (
//    double azimuth, double altitude)
//{
//  glm::vec3 res;
//  res[2] = -Cos(azimuth) * Cos(altitude);  // North
//  res[0] =  Sin(azimuth) * Cos(altitude);  // East
//  res[1] = -Sin(altitude); // Zenith
//  return res;
//}




glm::vec3 Sky::getSunPosition() const {
  glm::vec3 SunPos(
        sin(SunTheta) * sin(SunPhi) * m_radius,
        cos(SunTheta) * m_radius,
        sin(SunTheta) * cos(SunPhi) * m_radius
        );
  return SunPos;
}

void Sky::update(float /*dt*/, const glm::vec3 &cameraPosition) {

  //  if(IsRunningSimulation)
  //    TimeOfDay += dt*TimeScale;
  //  //TimeOfDay += dt;
  //  if (TimeOfDay > 3600 * 24) //next day?
  //  {
  //    JulianDay += 1.0f;
  //    TimeOfDay -= 3600 * 24;
  //  }

  float hour = TimeOfDay / 3600.0f;
  float solarTime = hour + 0.170 * sin( 4 * Pi<float>() * (JulianDay - 80) / 373 ) - 0.129 * sin( 2 * Pi<float>() * (JulianDay - 8) / 355) + 12 * (Latitude - Longitude) / Pi<float>();
  float declination = 0.4093 * sin( 2 * Pi<float>() * (JulianDay - 81) / 368 );

  SunTheta = Pi<float>()/2.5;//Pi<float>() / 2 - asin(sin(Latitude) * sin(declination) - cos(Latitude) * cos(declination) * cos(Pi<float>() * solarTime / 12));
  SunPhi   = Pi<float>();//atan( -cos(declination) * sin(Pi<float>() * solarTime / 12) / ((cos(Latitude) * cos(declination) - sin(Latitude) * sin(declination) * sin(Pi<float>() * solarTime / 12))));

  m_drawData.Uniforms.Vec3Uniforms["u_cameraPosition"] = cameraPosition;

  //m_drawData.Uniforms.Vec3Uniforms["u_sunPosition"] = getSunPosition();



  /*if (time > 3600 * 24) //next day?
  {
    time = 0;
  }
  else
    time+=100.0;
*/

  //m_clock.update(dt);


}

void Sky::toGPU(const ConfigurationManager& config, RenderDevice* device, RenderContext* context) {
  VertexBufferPtr vb = device->createVertexBuffer(m_vertices.data(), m_vertices.size()*sizeof(Vertex));

  std::string dataDir = config.getParam("DATA_DIR");

  std::set<std::string> defines;
  ShaderProgramPtr sp = device->createShaderProgramFromFile(
        (dataDir + "/shaders/skyshader.vsh").c_str(),
        (dataDir + "/shaders/skyshader.fsh").c_str(),
        defines);

  m_spBlackAndWhite = device->createShaderProgramFromFile(
        (dataDir + "/shaders/skyshaderblackandwhite.vsh").c_str(),
        (dataDir + "/shaders/skyshaderblackandwhite.fsh").c_str(),
        defines);


  int posAttr = sp->attributeLocation("posAttr");
  int texAttr = sp->attributeLocation("texAttr");
  VertexDescription vertexDesc =
  {
    {posAttr, sa::VertexDescriptionElement::Type::FLOAT, 3},
    {texAttr, sa::VertexDescriptionElement::Type::FLOAT, 2},
  };

  VertexArrayPtr vao = context->createVertexArray(vertexDesc, vb);

  IndexBufferPtr ib = device->createIndexBuffer(m_indices);



  m_drawData.BlendingFunction = Blending::Normal;
  m_drawData.PolygonDrawMode = PolygonMode::Fill;
  m_drawData.Primitive = DrawPrimitive::TriangleStrip;
  m_drawData.IsTwoSided = true;

  Image img((dataDir + "/textures/EarthClearSky2.png").c_str());
  unsigned char *data = new unsigned char [4 * img.getWidth()];
  for(unsigned int x = 0; x < img.getWidth(); ++x) {
    Image::Color c = img.getAveragedSlice(x);
    unsigned index = 4 * x;

    data[index+0] = c.b*255;
    data[index+1] = c.g*255;
    data[index+2] = c.r*255;
    data[index+3] = c.a*255;

  }


  m_drawData.TEX[0] = device->createTextureFromImage(img, Texture::ClampToEdge);

  Image fogImg(data, img.getWidth(), 1);
  m_drawData.TEX[1] = device->createTextureFromImage(fogImg, Texture::ClampToEdge);
  delete []data;
  data = nullptr;
  m_drawData.Current_SP = sp;
  m_drawData.VAO = vao;
  m_drawData.IB = ib;

  float aspect = context->width()/static_cast<float>(context->height());
  if(context->height() > context->width())
    aspect = context->height()/static_cast<float>(context->width());

  m_drawData.Uniforms.Matrix4Uniforms["u_skyProjectionMatrix"] = Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0), aspect, 0.01f, m_radius*2.0f);
  m_drawData.Uniforms.Matrix4Uniforms["u_modelMatrix"] = Matrix44T<float>::GetIdentity();
  m_drawData.Uniforms.FloatUniforms["u_time"] = 0.5;
  m_drawData.Uniforms.Vec4Uniforms["u_color"] = Vector4T<float>(1,0,0,0.5);
  m_drawData.Uniforms.Sampler2DUniforms["u_texture"] = 0;
  m_drawData.Uniforms.Sampler2DUniforms["u_fogColor"] = 1;
  context->resetCurrentState();
}

DrawData Sky::getDrawData(RenderPass renderPass)
{
  DrawData outData;
  outData = m_drawData;
  if(renderPass == RenderPass::SunLightShafts)
  {
    outData.Current_SP = m_spBlackAndWhite;
  }
  return outData;
}
}
}
