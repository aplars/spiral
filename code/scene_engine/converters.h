#pragma once

#include <scene_models/materialmodel.h>
#include <renderer_engine/texture.h>

namespace sa {
class Converters
{
public:
  Converters();
  static Texture::WrapMode convertWrapMode(MaterialModel::TextureMappingMode mode);
};
}
