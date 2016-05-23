#pragma once
#include <memory>
#include <set>
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "shaderprogram.h"
#include "texture.h"
#include "adt/lru.h"

namespace sa {
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;

/**
 * @brief The Device class is responsible for creating resources used by the graphics card for rendering.
 * Typical resources are shaders, vertices and textures. All resources i the Device can be shared between threads
 * but the class is not thread safe. Locks have to be applied by the programmer using this class.
 */
class RenderDevice
{
public:
  RenderDevice();
  ~RenderDevice();

  VertexBufferPtr createVertexBuffer(const void* data, int sizeInBytes) const;
  template <typename T>
  VertexBufferPtr createVertexBuffer(const std::vector<T>& data) const;

  IndexBufferPtr createIndexBuffer(const unsigned int* data, int numberOfIndices) const;
  IndexBufferPtr createIndexBuffer(const std::vector<unsigned int>& indices) const;

  ShaderProgramPtr createShaderProgram(const char* vertexProgram, const char* fragmantProgram, const std::set<std::string>& defines) const;

  ShaderProgramPtr createShaderProgramFromFile(const char* vertexProgram, const char* fragmantProgram, const std::set<std::string>& defines) const;

  ShaderProgramPtr createShaderProgram(const char* vertexProgram, const char* fragmantProgram, const char* geometryProgram, const std::set<std::string>& defines) const;

  ShaderProgramPtr createShaderProgramFromFile(const char* vertexProgram, const char* fragmantProgram, const char* geometryProgram, const std::set<std::string>& defines) const;

  TexturePtr createTextureFromImage(const Image& image, Texture::WrapMode wrapMode) const;

  TexturePtr createTextureFromFile(const char* filename, Texture::WrapMode wrapMode) const;

};


template <typename T>
VertexBufferPtr RenderDevice::createVertexBuffer(const std::vector<T>& data) const {
  return createVertexBuffer(
        (const void*)&data.front(),
        data.size()*sizeof(T));
}

}
