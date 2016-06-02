#include "renderdevice.h"
#include "image.h"

#include <functional>
#include <iostream>
#include <fstream>

namespace sa {
RenderDevice::RenderDevice()
{

}

RenderDevice::~RenderDevice()
{ }

VertexBufferPtr RenderDevice::createVertexBuffer(const void* data, int sizeInBytes) const {
  VertexBufferPtr vb = VertexBufferPtr(new VertexBuffer(sizeInBytes));
  vb->write(data);
  return vb;
}

IndexBufferPtr RenderDevice::createIndexBuffer(const unsigned int *data, int numberOfIndices) const
{
  IndexBufferPtr ib = IndexBufferPtr(new IndexBuffer(numberOfIndices));
  ib->write(data);
  return ib;
}

IndexBufferPtr RenderDevice::createIndexBuffer(const std::vector<unsigned int> &indices) const
{
  return createIndexBuffer(
        (const unsigned int*)&indices.front(),
        indices.size());
}

ShaderProgramPtr RenderDevice::createShaderProgram(const char* vertexProgram, const char* fragmantProgram, const std::set<std::string>& defines) const {
  ShaderProgramPtr sp = ShaderProgramPtr(new ShaderProgram(vertexProgram, fragmantProgram, defines));
  return sp;
}

std::string RenderDevice::readFromFile(const char* filePath) {
  std::string fileString;
  std::string line;
  std::ifstream file(filePath); // Open an input stream with the selected file
  if (file.is_open()) { // If the file opened successfully
    while (!file.eof()) { // While we are not at the end of the file
      getline(file, line); // Get the current line

        fileString.append(line); // Append the line to our file string
      fileString.append("\n"); // Appand a new line character
    }
    file.close(); // Close the file
  }

  return fileString; // Return our string

}

ShaderProgramPtr RenderDevice::createShaderProgramFromFile(const char* vertexProgram, const char* fragmantProgram, const std::set<std::string>& defines) const {

  return createShaderProgram(
        readFromFile(vertexProgram).c_str(),
        readFromFile(fragmantProgram).c_str(),
        defines);
}

ShaderProgramPtr RenderDevice::createShaderProgram(const char* vertexProgram, const char* fragmantProgram, const char* geometryProgram, const std::set<std::string>& defines) const {
  ShaderProgramPtr sp = ShaderProgramPtr(new ShaderProgram(vertexProgram, fragmantProgram, geometryProgram, defines));
  return sp;
}

ShaderProgramPtr RenderDevice::createShaderProgramFromFile(const char* vertexProgram, const char* fragmantProgram, const char* geometryProgram, const std::set<std::string>& defines) const {
  return createShaderProgram(
        readFromFile(vertexProgram).c_str(),
        readFromFile(fragmantProgram).c_str(),
        readFromFile(geometryProgram).c_str(),
        defines);
}

TexturePtr RenderDevice::createTextureFromImage(const Image& image, Texture::WrapMode wrapMode) const {
  if(image.isNull())
    return TexturePtr();
  TexturePtr t = TexturePtr(new Texture(image, true));
  t->setMinificationFilter(Texture::LinearMipMapLinear);
  t->setMagnificationFilter(Texture::Linear);
  t->setWrapMode(wrapMode);
  return t;
}

TexturePtr RenderDevice::createTextureFromFile(const char* filename, Texture::WrapMode wrapMode) const {
  Image img(filename);
  return createTextureFromImage(img, wrapMode);
}
}


