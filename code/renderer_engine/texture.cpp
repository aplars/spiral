#include "texture.h"
#include "image.h"
#include <QDebug>

namespace sa {
Texture::~Texture()
{
  m_glObject.destroy();
}

Texture::Texture()
  : m_glObject(QOpenGLTexture::Target2D)
{
  m_glObject.create();
}

Texture::Texture(const Image& image, bool generateMipMaps)
  : m_glObject(image.getQObject(), convertMipMapGeneration(generateMipMaps))
{
  m_glObject.setWrapMode(QOpenGLTexture::ClampToEdge);
}

Texture::Texture(TextureFormat textureFormat, int width, int height, PixelFormat pixelFormat, PixelType pixelType, const void* data)
  : m_glObject(QOpenGLTexture::Target2D)
{
  m_glObject.create();
  m_glObject.setSize(width, height);
  m_glObject.setFormat([](TextureFormat textureFormat) -> QOpenGLTexture::TextureFormat {
    switch(textureFormat) {
    case sa::Texture::RGB32F:
      return QOpenGLTexture::TextureFormat::RGB32F;
    case sa::Texture::RGBA32F:
      return QOpenGLTexture::TextureFormat::RGBA32F;
    case sa::Texture::Depth16:
      return QOpenGLTexture::TextureFormat::D16;
    case sa::Texture::Depth24:
      return QOpenGLTexture::TextureFormat::D24;
    case sa::Texture::Depth32:
      return QOpenGLTexture::TextureFormat::D32;
    default:
      return QOpenGLTexture::TextureFormat::RGB32F;
    }
  }(textureFormat));
  m_glObject.allocateStorage();

  if(data)
  {
    m_glObject.setData(0,
                       [](PixelFormat pixelFormat) -> QOpenGLTexture::PixelFormat {
      switch(pixelFormat)
      {
      case sa::Texture::RGB:
        return QOpenGLTexture::PixelFormat::RGB;
      case sa::Texture::BGR:
        return QOpenGLTexture::PixelFormat::BGR;
      case sa::Texture::RGBA:
        return QOpenGLTexture::PixelFormat::RGBA;
      case sa::Texture::BGRA:
        return QOpenGLTexture::PixelFormat::BGRA;
      case sa::Texture::Depth:
        return QOpenGLTexture::PixelFormat::Depth;
      default:
        return QOpenGLTexture::PixelFormat::RGB;

      }
    }(pixelFormat),
        [](PixelType pixelType) -> QOpenGLTexture::PixelType {
      switch(pixelType)
      {
      case sa::Texture::Int8:
        return QOpenGLTexture::Int8;
      case sa::Texture::UInt8:
        return QOpenGLTexture::UInt8;
      case sa::Texture::Int16:
        return QOpenGLTexture::Int16;
      case sa::Texture::UInt16:
        return QOpenGLTexture::UInt16;
      case sa::Texture::Int32:
        return QOpenGLTexture::Int32;
      case sa::Texture::UInt32:
        return QOpenGLTexture::UInt32;
      case sa::Texture::Float32:
        return QOpenGLTexture::Float32;
      default:
        return QOpenGLTexture::Int8;
      }
    }(pixelType),
        data
        );
  }
}

void Texture::setMagnificationFilter(Filter filter) {
  m_glObject.setMagnificationFilter(convertFilter(filter));
}

void Texture::setMinificationFilter(Filter filter) {
  m_glObject.setMinificationFilter(convertFilter(filter));
}

void Texture::setWrapMode(WrapMode wrapMode) {
  m_glObject.setWrapMode(convertWrapMode(wrapMode));
}

void Texture::bind(int unit) {
  m_glObject.bind(unit);
}

void Texture::bind() {
  m_glObject.bind();
}

void Texture::release() {
  m_glObject.release();
}
}

