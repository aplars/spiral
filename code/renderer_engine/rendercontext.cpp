#include "rendercontext.h"
namespace sa {
RenderContext::RenderContext()
{

}

RenderContext::~RenderContext()
{ }

bool RenderContext::create(GLuint defaultFB, unsigned int width, unsigned int height)
{
  m_defaultFB = defaultFB;
  m_width = width;
  m_height = height;
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  return true;
}

void RenderContext::destroy() {
  m_currentIB = nullptr;
  m_currentVAO = nullptr;
  m_currentSP = nullptr;
  for(int i = 0; i < DrawData::NUM_TEX_UNITS; ++i)
    m_currentTEX[i] = nullptr;
}

void RenderContext::setViewport(unsigned int w, unsigned int h) {
  glViewport(0, 0, w, h);
  m_width = w;
  m_height = h;
}

void RenderContext::setCullFace(CullFace cf) {
  GLenum mode = GL_FRONT;
  if(cf == CullFace::Back)
    mode = GL_BACK;
  glCullFace(mode);
}

VertexArrayPtr RenderContext::createVertexArray(VertexDescription description, VertexBufferPtr buffer) {
  VertexArrayPtr vao = VertexArrayPtr(new VertexArray());
  vao->bind();
  buffer->bind();
  int offset = 0;
  for(VertexDescriptionElement e : description) {
    if(e.shaderAttribLocation() != -1) {
      glVertexAttribPointer(e.shaderAttribLocation(), e.numberOfComponents(), (int)e.type(), GL_FALSE, description.sizeInBytes(), (char*)nullptr + offset);
      glEnableVertexAttribArray(e.shaderAttribLocation());
    }
    offset += e.sizeInBytes();
  }
  buffer->release();
  vao->release();
  return vao;
}

RenderDepthToTexturePtr RenderContext::createRenderDepthToTexture(unsigned int width, unsigned int height) {
  return RenderDepthToTexturePtr(new RenderDepthToTexture(m_defaultFB, width, height));
}

void RenderContext::clear() {
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
}

void RenderContext::draw(DrawData drawData) {
  //glClearColor(0.3, 0.3, 0.3, 1);

  if(static_cast<int>(drawData.IsTwoSided) != m_currentIsTwoSided) {
    if(drawData.IsTwoSided)
      glDisable(GL_CULL_FACE);
    else
      glEnable(GL_CULL_FACE);

    m_currentIsTwoSided = static_cast<int>(drawData.IsTwoSided);
  }

  if(drawData.BlendingFunction == Blending::Normal) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }
  else if (drawData.BlendingFunction == Blending::Addative)
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
  }
  else {
    glDisable(GL_BLEND);
  }
  //Bind the texture units
  for(int i = 0; i < DrawData::NUM_TEX_UNITS; ++i)
  {
    if(drawData.TEX[i] != m_currentTEX[i])
    {
      if(drawData.TEX[i]) {
        drawData.TEX[i]->bind(i);
        drawData.TEX[i]->bind();
        m_currentTEX[i] = drawData.TEX[i];
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


      }
    }
  }

  //Bind shader program if changed
  if(drawData.SP != m_currentSP)
  {
    if(drawData.SP)
      drawData.SP->bind();
    m_currentSP = drawData.SP;
  }
  //Set shader uniforms. This is usually cheap operation and therefore all uniforms
  //are written. Not just the ones that have changed.
  setUniforms(m_currentSP, drawData.Uniforms);

  //Bind the vertex array if changed.
  if(drawData.VAO != m_currentVAO)
  {
    if(drawData.VAO)
      drawData.VAO->bind();
    m_currentVAO = drawData.VAO;
  }

  if(drawData.IB)
  {
    //Bind the index array if changed.
    if(drawData.IB != m_currentIB)
    {
      drawData.IB->bind();
      m_currentIB = drawData.IB;
    }
    if(drawData.IsVisible)
      glDrawElements(GL_TRIANGLES, drawData.IB->numberOfIndices(), GL_UNSIGNED_INT, 0);
  }
  else {
    //No index buffer. Draw using the number of triangles attribute instead.
    if(drawData.IsVisible)
      glDrawArrays(GL_TRIANGLES, 0, drawData.NumberOfTrianglesToDraw);
  }
}

void RenderContext::draw(const DrawDataList& drawDataList, ShaderUniforms shaderUniforms) {
  //Set all global uniforms.
  for(ShaderProgramPtr shader : drawDataList.shaders()) {
    //Bind shader program if changed
    if(shader) {
      shader->bind();
      setUniforms(shader, shaderUniforms);
    }
  }
  m_currentSP = nullptr;
  //Draw all objects.
  for(DrawDataList::value_type dd : drawDataList) {
    draw(dd);
  }
}

void RenderContext::setUniforms(ShaderProgramPtr shader, ShaderUniforms uniforms) {
  if(shader == nullptr) return;

  for(FloatUniformsMap::value_type uniform : uniforms.FloatUniforms)
  {
    shader->setUniformValue(uniform.first, uniform.second);
  }
  for(Vec3UniformsMap::value_type uniform : uniforms.Vec3Uniforms)
  {
    shader->setUniformValue(uniform.first, uniform.second);
  }
  for(Vec4UniformsMap::value_type uniform : uniforms.Vec4Uniforms)
  {
    shader->setUniformValue(uniform.first, uniform.second);
  }
  for(Matrix4UniformsMap::value_type uniform : uniforms.Matrix4Uniforms)
  {
    shader->setUniformValue(uniform.first, uniform.second);
  }
  for(Sampler2DUniformsMap::value_type uniform : uniforms.Sampler2DUniforms)
  {
    shader->setUniformValue(uniform.first, uniform.second);
  }
  for(FloatArrayUniformsMap::value_type uniform : uniforms.FloatArrayUniforms)
  {
    shader->setUniformValueArray(uniform.first, uniform.second);
  }
  for(Matrix4ArrayUniformsMap::value_type uniform : uniforms.Matrix4ArrayUniforms)
  {
    shader->setUniformValueArray(uniform.first, uniform.second);
  }
  for(Sampler2DArrayUniformsMap::value_type uniform : uniforms.Sampler2DArrayUniforms)
  {
    shader->setUniformValueArray(uniform.first, uniform.second);
  }
}
}

