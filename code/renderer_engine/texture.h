#pragma once
#include <QOpenGLTexture>
#include <memory>
namespace sa {
class Image;
class Texture
{
public:
  enum Filter {
    Nearest,
    Linear,
    NearestMipMapNearest,
    NearestMipMapLinear,
    LinearMipMapNearest,
    LinearMipMapLinear
  };

  enum WrapMode {
      Repeat         = 0x2901, // GL_REPEAT
      MirroredRepeat = 0x8370, // GL_MIRRORED_REPEAT
      ClampToEdge    = 0x812F, // GL_CLAMP_TO_EDGE
      ClampToBorder  = 0x812D  // GL_CLAMP_TO_BORDER
  };

  enum PixelFormat {
    RGB,
    BGR,
    RGBA,
    BGRA,
    Depth
  };

  enum PixelType {
    Int8,
    UInt8,
    Int16,
    UInt16,
    Int32,
    UInt32,
    Float32
  };

  enum TextureFormat {
    RGB32F,
    RGBA32F,
    Depth16,
    Depth24,
    Depth32
  };

  ~Texture();
  Texture();
  Texture(const Image& image, bool generateMipMaps);
  Texture(TextureFormat textureFormat, int width, int height, PixelFormat pixelFormat, PixelType pixelType, const void* data);
  void setMagnificationFilter(Filter filter);
  void setMinificationFilter(Filter filter);
  void setWrapMode(WrapMode wrapMode);
  void bind(int unit);
  void bind();
  void release();
  GLuint glTexId() const { return m_glObject.textureId(); }
private:
  Texture(const Texture&);
  const Texture& operator=(const Texture&);

  static QOpenGLTexture::MipMapGeneration convertMipMapGeneration(bool generateMipMap) {
    if(generateMipMap) return QOpenGLTexture::MipMapGeneration::GenerateMipMaps;
    return QOpenGLTexture::MipMapGeneration::DontGenerateMipMaps;
  }

  static QOpenGLTexture::Filter convertFilter(Filter filter) {
    QOpenGLTexture::Filter qFilter;
    switch(filter) {
    case sa::Texture::Nearest:
      qFilter = QOpenGLTexture::Filter::Nearest;
      break;
    case sa::Texture::Linear:
      qFilter = QOpenGLTexture::Filter::Linear;
      break;
    case sa::Texture::NearestMipMapNearest:
      qFilter = QOpenGLTexture::Filter::NearestMipMapNearest;
      break;
    case sa::Texture::NearestMipMapLinear:
      qFilter = QOpenGLTexture::Filter::NearestMipMapLinear;
      break;
    case sa::Texture::LinearMipMapNearest:
      qFilter = QOpenGLTexture::Filter::LinearMipMapNearest;
      break;
    case sa::Texture::LinearMipMapLinear:
      qFilter = QOpenGLTexture::Filter::LinearMipMapLinear;
      break;
    }
    return qFilter;
  }

  static QOpenGLTexture::WrapMode convertWrapMode(WrapMode wm) {
    switch(wm) {
    case sa::Texture::MirroredRepeat:
      return QOpenGLTexture::WrapMode::MirroredRepeat;
    case sa::Texture::ClampToEdge:
      return QOpenGLTexture::WrapMode::ClampToEdge;
    case sa::Texture::ClampToBorder:
      return QOpenGLTexture::WrapMode::ClampToBorder;
    case Texture::Repeat:
      return QOpenGLTexture::WrapMode::Repeat;
      break;
    default:
      return QOpenGLTexture::WrapMode::MirroredRepeat;
    }
  }


  QOpenGLTexture m_glObject;
};
typedef std::shared_ptr<Texture> TexturePtr;
}

