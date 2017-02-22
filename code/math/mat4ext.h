#ifndef MAT4EXT_H
#define MAT4EXT_H
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Matrix44T.h"

namespace sa {
class Mat4ext
{
public:
  static glm::mat4 toMat4(const Matrix44T<float>& m) {
    return glm::make_mat4(m.GetConstPtr());
  }

  static Matrix44T<float> fromMat4(const glm::mat4& m) {
    float arr[16];
    const float *ptr = glm::value_ptr(m);
    for(int i = 0; i < 16; i++) {
      arr[i] = ptr[i];
    }
    return Matrix44T<float>(arr);
  }

};
}
#endif // MAT4EXT_H
