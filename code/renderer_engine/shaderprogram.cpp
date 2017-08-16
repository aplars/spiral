#include "shaderprogram.h"
#include <glm/gtc/type_ptr.hpp>

sa::ShaderProgram::~ShaderProgram()
{
  m_glObject.removeAllShaders();
}

sa::ShaderProgram::ShaderProgram(const char* vertexProgram, const char* fragmentProgram, const std::set<std::string>& defines)
  : m_vsName(vertexProgram)
  , m_fsName(fragmentProgram)
{
  //Add the defines to the shaders.
  std::string vertexProgramWithDefines;
  for(const std::string define : defines) {
    vertexProgramWithDefines += ("#define " + define + "\n");
  }
  vertexProgramWithDefines += vertexProgram;

  std::string fragmentProgramWithDefines;
  for(const std::string define : defines) {
    fragmentProgramWithDefines += ("#define " + define + "\n");
  }
  fragmentProgramWithDefines += fragmentProgram;


  m_glObject.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexProgramWithDefines.c_str());
  m_glObject.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentProgramWithDefines.c_str());
  if(!m_glObject.link()) {
    qDebug() << "Failed to ling shader: " << vertexProgram << " " << fragmentProgram;
  }

  m_glObject.bind();
  initializeOpenGLFunctions();
  cacheUniforms();
  m_glObject.release();
}

sa::ShaderProgram::ShaderProgram(const char* vertexProgram, const char* fragmentProgram, const char* geometryProgram, const std::set<std::string>& defines)
  : m_vsName(vertexProgram)
  , m_fsName(fragmentProgram)
  , m_gsName(geometryProgram)
{
  //Add the defines to the shaders.
  std::string vertexProgramWithDefines;
  for(const std::string define : defines) {
    vertexProgramWithDefines += ("#define " + define + "\n");
  }
  vertexProgramWithDefines += vertexProgram;

  std::string fragmentProgramWithDefines;
  for(const std::string define : defines) {
    fragmentProgramWithDefines += ("#define " + define + "\n");
  }
  fragmentProgramWithDefines += fragmentProgram;

  std::string geometryProgramWithDefines;
  for(const std::string define : defines) {
    geometryProgramWithDefines += ("#define " + define + ";\n");
  }
  geometryProgramWithDefines += geometryProgram;

  m_glObject.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexProgramWithDefines.c_str());
  m_glObject.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentProgramWithDefines.c_str());
  m_glObject.addShaderFromSourceCode(QOpenGLShader::Geometry, geometryProgramWithDefines.c_str());
  m_glObject.link();

  m_glObject.bind();
  initializeOpenGLFunctions();
  cacheUniforms();
  m_glObject.release();
}

void sa::ShaderProgram::bind() {
  m_glObject.bind();
}

void sa::ShaderProgram::release()
{
  m_glObject.release();
}

void sa::ShaderProgram::link() {
  m_glObject.link();
}

int sa::ShaderProgram::attributeLocation(const char* name) const {
  int attribLocation = m_glObject.attributeLocation(name);
  return attribLocation;
}

void sa::ShaderProgram::bindAttributeLocation(const char* name, int location) {
  if(location == -1) {
    qDebug() << "ShaderProgram: Attribute " << name << " not found";
  }
  else {
    m_glObject.bindAttributeLocation(name, location);
  }
}

int sa::ShaderProgram::uniformLocation(const char* name) const {
  std::map<std::string, int>::const_iterator it = m_uniformsCache.find(name);
  if(it != m_uniformsCache.end())
    return it->second;

  qDebug() << "ShaderProgram: Uniform " << name << " not found";
  return -1;
}

void sa::ShaderProgram::setUniformValue(const std::string &location, float value) {
  m_glObject.setUniformValue(location.c_str(), value);
}

void sa::ShaderProgram::setUniformValue(const std::string &location, const glm::vec2 &value) {
  m_glObject.setUniformValue(location.c_str(), value[0], value[1]);
}

void sa::ShaderProgram::setUniformValue(const std::string &location, const glm::vec3 &value) {
  m_glObject.setUniformValue(location.c_str(), value[0], value[1], value[2]);
}

void sa::ShaderProgram::setUniformValue(const std::string &location, const glm::vec4 &value) {
  m_glObject.setUniformValue(location.c_str(), value[0], value[1], value[2], value[3]);
}

void sa::ShaderProgram::setUniformValue(const std::string &location, const glm::mat4 &value)
{
  m_glObject.bind();
  glUniformMatrix4fv(m_glObject.uniformLocation(location.c_str()), 1, false, glm::value_ptr(value));
}

void sa::ShaderProgram::setUniformValue(const std::string &location, const float value[4][4]) {
  m_glObject.setUniformValue(location.c_str(), value);
}

void sa::ShaderProgram::setUniformValue(const std::string &location, int value) {
  m_glObject.setUniformValue(location.c_str(), value);
}

void sa::ShaderProgram::setUniformValue(const std::string &location, unsigned int value) {
  m_glObject.setUniformValue(location.c_str(), value);
}

void sa::ShaderProgram::setUniformValueArray(const std::string &location, const std::vector<float>& values) {
  m_glObject.setUniformValueArray(location.c_str(), values.data(), values.size(), 1);
}

void sa::ShaderProgram::setUniformValueArray(const std::string &location, const std::vector<glm::mat4> &values) {
  m_glObject.bind();
  const glm::mat4& ff = values[0];
  glUniformMatrix4fv(m_glObject.uniformLocation(location.c_str()), values.size(), false, glm::value_ptr(ff));
}

void sa::ShaderProgram::setUniformValueArray(const std::string &location, const std::vector<unsigned int>& values) {
  m_glObject.setUniformValueArray(location.c_str(), values.data(), values.size());
}

void sa::ShaderProgram::cacheUniforms() {
  GLint numUniforms;
  glGetProgramiv(m_glObject.programId(), GL_ACTIVE_UNIFORMS, &numUniforms);
  GLint uniformNameMaxLen=0;
  glGetProgramiv(m_glObject.programId(), GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformNameMaxLen);
  GLchar* uniformName = new GLchar[uniformNameMaxLen];
  for (int i = 0  ; i < numUniforms ; i++)
  {
    GLsizei size;
    GLenum type;
    glGetActiveUniform(m_glObject.programId(), i, uniformNameMaxLen, NULL, &size, &type, uniformName);
    unsigned int uniformLocation = glGetUniformLocation(m_glObject.programId(), uniformName);
    std::string sName(uniformName);
    std::string sReplace = "";
    std::string sSubStr = "[0]";
    int findIt = sName.find(sSubStr);
    m_uniformsCache[sName] = uniformLocation;
    if(findIt != -1) {
      sName.replace(findIt ,sSubStr.length(),sReplace);
      m_uniformsCache[sName] = uniformLocation;
    }
  }
  delete []uniformName;
  uniformName = NULL;

}

