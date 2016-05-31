#pragma once

#include <math/Matrix44T.h>
#include <QOpenGLShaderProgram>
#include <memory>
#include <set>
#include <QOpenGLFunctions>

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
  void setUniformValue(const std::string& location, const Vector3T<float> &value);
  void setUniformValue(const std::string& location, const Vector4T<float> &value);
  void setUniformValue(const std::string& location, const Matrix44T<float>& value);
  void setUniformValue(const std::string& location, const float value[4][4]);
  void setUniformValue(const std::string& location, unsigned int value);

  void setUniformValueArray(const std::string& location, const std::vector<float>& values);
  void setUniformValueArray(const std::string& location, const std::vector<unsigned int>& values);
  void setUniformValueArray(const std::string& location, const std::vector<Matrix44T<float>>& values);
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
