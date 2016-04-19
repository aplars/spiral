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
private:
  GLuint m_defaultFB;

  GLuint m_glObject;
  TexturePtr m_texture;
  TexturePtr m_depthTexture;
  unsigned int m_width;
  unsigned int m_height;
};
typedef std::shared_ptr<RenderDepthToTexture> RenderDepthToTexturePtr;
}
