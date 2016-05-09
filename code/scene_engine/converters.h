#ifndef CONVERTERS_H
#define CONVERTERS_H
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
#endif // CONVERTERS_H
