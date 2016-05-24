#pragma once
#include "drawdata.h"
#include "vertexarray.h"
#include "vertexbuffer.h"
#include "framebuffer.h"
#include "vertexdescription.h"
#include <QOpenGLFunctions>
#include <memory>

namespace sa {

class RenderContext : protected QOpenGLFunctions
{
public:
  enum CullFace {
    Front,
    Back
  };

  RenderContext();
  ~RenderContext();
  bool create(GLuint defaultFB, unsigned int width, unsigned int height);
  void destroy();
  unsigned int width() const {
    return m_width;
  }
  unsigned int height() const {
    return m_height;
  }

  void setViewport(unsigned int w, unsigned int h);
  void setCullFace(CullFace cf);

  VertexArrayPtr createVertexArray(VertexDescription description, VertexBufferPtr buffer);
  RenderDepthToTexturePtr createRenderDepthToTexture(unsigned int width, unsigned int height);
  void clear();
  void draw(DrawData drawData);
  void draw(const DrawDataList& drawDataList, ShaderUniforms shaderUniforms);

  void makeDirty() {
    m_currentIB = nullptr;
    m_currentVAO = nullptr;
    m_currentSP = nullptr;
    for(int i = 0; i < DrawData::NUM_TEX_UNITS; ++i)
      m_currentTEX[i] = nullptr;
  }

  void resetCurrentState() {
    if(m_currentIB)
      m_currentIB->bind();
    if(m_currentVAO)
        m_currentVAO->bind();
    if(m_currentSP)
      m_currentSP->bind();
    for(int i = 0; i < DrawData::NUM_TEX_UNITS; ++i)
      if(m_currentTEX[i])
        m_currentTEX[i]->bind();
  }

private:
  void setUniforms(ShaderProgramPtr shader, ShaderUniforms uniforms);

  GLuint m_defaultFB;
  unsigned int m_width;
  unsigned int m_height;
  int m_currentIsTwoSided = -1;
  IndexBufferPtr m_currentIB;
  VertexArrayPtr m_currentVAO;
  ShaderProgramPtr m_currentSP;
  TexturePtr m_currentTEX[DrawData::NUM_TEX_UNITS];
};
}
