#include "shaderprogram.h"

sa::ShaderProgram::~ShaderProgram()
{
  m_glObject.removeAllShaders();
}

sa::ShaderProgram::ShaderProgram(const char* vertexProgram, const char* fragmentProgram, const std::set<std::string>& defines)
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
  m_glObject.link();

  m_glObject.bind();
  initializeOpenGLFunctions();
  cacheUniforms();
  m_glObject.release();

}

sa::ShaderProgram::ShaderProgram(const char* vertexProgram, const char* fragmentProgram, const char* geometryProgram, const std::set<std::string>& defines)
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

int sa::ShaderProgram::attributeLocation(const char* name) const {
  return m_glObject.attributeLocation(name);
}

int sa::ShaderProgram::uniformLocation(const char* name) const {
  std::map<std::string, int>::const_iterator it = m_uniformsCache.find(name);
  if(it != m_uniformsCache.end())
    return it->second;
  return -1;
}

void sa::ShaderProgram::setUniformValue(int location, float value) {
  m_glObject.setUniformValue(location, value);
}

void sa::ShaderProgram::setUniformValue(int location, const Vector3T<float> &value) {
  m_glObject.setUniformValue(location, value[0], value[1], value[2]);
}

void sa::ShaderProgram::setUniformValue(int location, const Vector4T<float> &value) {
  m_glObject.setUniformValue(location, value[0], value[1], value[2], value[3]);
}

void sa::ShaderProgram::setUniformValue(int location, const Matrix44T<float> &value)
{
  m_glObject.bind();
  glUniformMatrix4fv(location, 1, false, value.GetConstPtr());
}

void sa::ShaderProgram::setUniformValue(int location, const float value[4][4]) {
  m_glObject.setUniformValue(location, value);
}

void sa::ShaderProgram::setUniformValue(int location, unsigned int value) {
  m_glObject.setUniformValue(location, value);
}

void sa::ShaderProgram::setUniformValueArray(int location, const std::vector<float>& values) {
  m_glObject.setUniformValueArray(location, values.data(), values.size(), 1);
}

void sa::ShaderProgram::setUniformValueArray(int location, const std::vector<Matrix44T<float>>& values) {
  m_glObject.bind();
  glUniformMatrix4fv(location, values.size(), false, values.data()->GetConstPtr());
}

void sa::ShaderProgram::setUniformValueArray(int location, const std::vector<unsigned int>& values) {
  m_glObject.setUniformValueArray(location, values.data(), values.size());
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
