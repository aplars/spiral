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

  void setUniformValue(int location, float value);
  void setUniformValue(int location, const Vector3T<float> &value);
  void setUniformValue(int location, const Vector4T<float> &value);
  void setUniformValue(int location, const Matrix44T<float>& value);
  void setUniformValue(int location, const float value[4][4]);
  void setUniformValue(int location, unsigned int value);

  void setUniformValueArray(int location, const std::vector<float>& values);
  void setUniformValueArray(int location, const std::vector<unsigned int>& values);
  void setUniformValueArray(int location, const std::vector<Matrix44T<float>>& values);
private:
  void cacheUniforms();
  mutable QOpenGLShaderProgram m_glObject;
  std::map<std::string, int> m_uniformsCache;
};

typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;
}
