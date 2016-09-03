#pragma once
#include <QOpenGLFunctions>
#include "texture.h"

namespace sa {
class RenderDepthToTexture : public QOpenGLFunctions
{
public:
  ~RenderDepthToTexture();
  RenderDepthToTexture(GLuint defaultFB, unsigned int width, unsigned int height);
  void bind();
  void release();
  TexturePtr getTexture() const;
  TexturePtr getDepthTexture() const;
  unsigned int getWidth() const;

  unsigned int getHeight() const;

private:
  GLuint m_defaultFB;
  unsigned int m_width;
  unsigned int m_height;

  GLuint m_glObject;
  TexturePtr m_texture;
  TexturePtr m_depthTexture;
};
typedef std::shared_ptr<RenderDepthToTexture> RenderDepthToTexturePtr;

class RenderToTexture : public QOpenGLFunctions
{
public:
  ~RenderToTexture();
  RenderToTexture(GLuint defaultFB, unsigned int width, unsigned int height);
  void bind();
  void release();
  TexturePtr getTexture() const;

  unsigned int getWidth() const;

  unsigned int getHeight() const;

private:
  GLuint m_defaultFB;
  unsigned int m_width;
  unsigned int m_height;

  GLuint m_glObject;
  TexturePtr m_texture;
};
typedef std::shared_ptr<RenderToTexture> RenderToTexturePtr;
}
