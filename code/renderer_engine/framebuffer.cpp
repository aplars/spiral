#include "framebuffer.h"
#include <QDebug>

namespace sa {
RenderDepthToTexture::~RenderDepthToTexture()
{
  glDeleteFramebuffers(1, &m_glObject);
}

RenderDepthToTexture::RenderDepthToTexture(GLuint defaultFB, unsigned int width, unsigned int height)
  : m_defaultFB(defaultFB)
  , m_width(width)
  , m_height(height)
  , m_texture(new Texture())
  , m_depthTexture(new Texture())
{
  initializeOpenGLFunctions();
//  GLuint texId = m_texture->glTexId();
  GLuint depthTexId = m_depthTexture->glTexId();

//  glBindTexture(GL_TEXTURE_2D, texId);
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, depthTexId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_2D, 0);

  GLuint theNameRenderBuffer;
  glGenFramebuffers(1, &m_glObject);

//  glGenRenderbuffers(1, &theNameRenderBuffer);
//  glBindRenderbuffer(GL_RENDERBUFFER, theNameRenderBuffer);
//  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

  glBindFramebuffer(GL_FRAMEBUFFER, m_glObject);

  //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, theNameRenderBuffer);

  //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexId, 0);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    qDebug() << "glCheckFramebufferStatus FAILED";
  }
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFB);
}

void RenderDepthToTexture::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_glObject);
  glViewport(0,0,m_width, m_height);
}

void RenderDepthToTexture::release() {
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFB);
}

TexturePtr RenderDepthToTexture::getTexture() const {
  return m_texture;
}

TexturePtr RenderDepthToTexture::getDepthTexture() const {
  return m_depthTexture;
}
//////////////////////////////////////////////////////////////////////


RenderToTexture::~RenderToTexture()
{
  glDeleteFramebuffers(1, &m_glObject);
}

RenderToTexture::RenderToTexture(GLuint defaultFB, unsigned int width, unsigned int height)
  : m_defaultFB(defaultFB)
  , m_width(width)
  , m_height(height)
  , m_texture(new Texture())
{
  initializeOpenGLFunctions();
    GLuint texId = m_texture->glTexId();

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint theNameRenderBuffer;
    glGenFramebuffers(1, &m_glObject);

    glGenRenderbuffers(1, &theNameRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, theNameRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, m_glObject);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, theNameRenderBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      qDebug() << "glCheckFramebufferStatus FAILED";
    }

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFB);
}

void RenderToTexture::bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_glObject);
  glViewport(0,0,m_width, m_height);
}

void RenderToTexture::release()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFB);
}

TexturePtr RenderToTexture::getTexture() const
{
  return m_texture;
}
}
