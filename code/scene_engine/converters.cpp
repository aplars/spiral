#include "converters.h"
namespace sa {
Converters::Converters()
{

}

Texture::WrapMode Converters::convertWrapMode(MaterialModel::TextureMappingMode mode) {
  switch(mode) {
  case sa::MaterialModel::Repeat:
    return Texture::WrapMode::Repeat;
  case sa::MaterialModel::MirroredRepeat:
    return Texture::WrapMode::MirroredRepeat;
  case sa::MaterialModel::ClampToEdge:
    return Texture::WrapMode::ClampToEdge;
  case sa::MaterialModel::ClampToBorder:
    return Texture::WrapMode::ClampToBorder;
  default:
    return Texture::WrapMode::Repeat;
  }
}
}
