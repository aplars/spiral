#ifndef VEC3EXT_H
#define VEC3EXT_H
#include <glm/vec3.hpp>
namespace sa {
class Vec3ext
{
public:
  Vec3ext();
  static glm::vec3 MinVec3(const glm::vec3& a, const glm::vec3& b) {
    glm::vec3 c;
    for(unsigned int i = 0; i < 3; ++i) {
      c[i] = Min(a[i], b[i]);
    }
    return c;
  }

  static glm::vec3 MaxVec3(const glm::vec3& a, const glm::vec3& b) {
    glm::vec3 c;
    for(unsigned int i = 0; i < 3; ++i) {
      c[i] = Max(a[i], b[i]);
    }
    return c;
  }

};
}
#endif // VEC3EXT_H
