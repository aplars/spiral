#pragma once
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shaderprogram.h"
#include "texture.h"
#include <map>

namespace sa {

typedef std::map<std::string, float> FloatUniformsMap;
typedef std::map<std::string, Vector3T<float> > Vec3UniformsMap;
typedef std::map<std::string, Vector4T<float> > Vec4UniformsMap;
typedef std::map<std::string, Matrix44T<float> > Matrix4UniformsMap;
typedef std::map<std::string, unsigned int> Sampler2DUniformsMap;
typedef std::map<std::string, std::vector<float>> FloatArrayUniformsMap;
typedef std::map<std::string, std::vector<Matrix44T<float>>> Matrix4ArrayUniformsMap;
typedef std::map<std::string, std::vector<unsigned int>> Sampler2DArrayUniformsMap;

struct DrawData {
  bool IsVisible = true;
  bool IsTwoSided = true;
  bool IsAlphaBlended = false;
  static const int NUM_TEX_UNITS = 8;
  FloatUniformsMap FloatUniforms;
  Vec3UniformsMap Vec3Uniforms;
  Vec4UniformsMap Vec4Uniforms;
  Matrix4UniformsMap Matrix4Uniforms;
  Sampler2DUniformsMap Sampler2DUniforms;
  FloatArrayUniformsMap FloatArrayUniforms;
  Matrix4ArrayUniformsMap Matrix4ArrayUniforms;
  Sampler2DArrayUniformsMap Sampler2DArrayUniforms;
  IndexBufferPtr IB;
  VertexArrayPtr VAO;
  ShaderProgramPtr SP;
  ShaderProgramPtr SSP;
  TexturePtr TEX[NUM_TEX_UNITS];
  unsigned int NumberOfTrianglesToDraw;
};
}
