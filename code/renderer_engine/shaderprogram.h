#pragma once

#include <QOpenGLShaderProgram>
#include <memory>
#include <set>
#include <QOpenGLFunctions>
#include <glm/mat4x4.hpp>
namespace sa {
class ShaderProgram : QOpenGLFunctions
{
public:
  virtual ~ShaderProgram();
  ShaderProgram(const char* vertexProgram, const char* fragmentProgram, const std::set<std::string>& defines);
  ShaderProgram(const char* vertexProgram, const char* fragmentProgram, const char* geometryProgram, const std::set<std::string>& defines);
  void bind();
  void release();
  void link();

  int attributeLocation(const char* name) const;
  void bindAttributeLocation(const char* name, int location);
  int uniformLocation(const char* name) const;

  void setUniformValue(const std::string& location, float value);
  void setUniformValue(const std::string& location, const glm::vec2 &value);
  void setUniformValue(const std::string& location, const glm::vec3 &value);
  void setUniformValue(const std::string& location, const glm::vec4 &value);
  void setUniformValue(const std::string& location, const glm::mat4 &value);
  void setUniformValue(const std::string& location, const float value[4][4]);
  void setUniformValue(const std::string& location, unsigned int value);

  void setUniformValueArray(const std::string& location, const std::vector<float>& values);
  void setUniformValueArray(const std::string& location, const std::vector<unsigned int>& values);
  void setUniformValueArray(const std::string& location, const std::vector<glm::mat4>& values);
private:
  void cacheUniforms();
  std::string m_vsName;
  std::string m_fsName;
  std::string m_gsName;
  mutable QOpenGLShaderProgram m_glObject;
  std::map<std::string, int> m_uniformsCache;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
}
