#ifndef SHADERUNIFORMS_H
#define SHADERUNIFORMS_H
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace sa {
typedef std::map<std::string, float> FloatUniformsMap;
typedef std::map<std::string, glm::vec2> Vec2UniformsMap;
typedef std::map<std::string, glm::vec3> Vec3UniformsMap;
typedef std::map<std::string, glm::vec4> Vec4UniformsMap;
typedef std::map<std::string, glm::mat4> Matrix4UniformsMap;
typedef std::map<std::string, unsigned int> Sampler2DUniformsMap;
typedef std::map<std::string, std::vector<float>> FloatArrayUniformsMap;
typedef std::map<std::string, std::vector<glm::mat4>> Matrix4ArrayUniformsMap;
typedef std::map<std::string, std::vector<unsigned int>> Sampler2DArrayUniformsMap;

struct ShaderUniforms {
  FloatUniformsMap FloatUniforms;
  Vec2UniformsMap Vec2Uniforms;
  Vec3UniformsMap Vec3Uniforms;
  Vec4UniformsMap Vec4Uniforms;
  Matrix4UniformsMap Matrix4Uniforms;
  Sampler2DUniformsMap Sampler2DUniforms;
  FloatArrayUniformsMap FloatArrayUniforms;
  Matrix4ArrayUniformsMap Matrix4ArrayUniforms;
  Sampler2DArrayUniformsMap Sampler2DArrayUniforms;
};
}
#endif // SHADERUNIFORMS_H
