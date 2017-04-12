#ifndef MAT4EXT_H
#define MAT4EXT_H
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <vector>
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

  static std::vector<Matrix44T<float>> fromMat4(const std::vector<glm::mat4>& vm) {
    std::vector<Matrix44T<float>> vm44;
    for(auto m : vm) {
      vm44.push_back(fromMat4(m));
    }
    return vm44;
  }

  enum ClipPlane {
    Left = 0, Right, Top, Bottom, Near, Far
  };

  static PlaneT<float> GetClipPlane(const glm::mat4& mat, ClipPlane plane)
  {
    float m = 0.0;
    short cola;
    switch(plane)
    {
    case Left:
      m = 1.0;
      cola = 0;
      break;
    case Right:
      m = -1.0;
      cola = 0;
      break;
    case Top:
      m = -1.0;
      cola = 1;
      break;
    case Bottom:
      m = 1.0;
      cola = 1;
      break;
    case Near:
      m = 1.0;
      cola = 2;
      break;
    case Far:
      m = -1.0;
      cola = 2;
      break;

    }
    float a = glm::column(mat, 0)[3] + m * glm::column(mat, 0)[cola];
    float b = glm::column(mat, 1)[3] + m * glm::column(mat, 1)[cola];
    float c = glm::column(mat, 2)[3] + m * glm::column(mat, 2)[cola];
    float d = glm::column(mat, 3)[3] + m * glm::column(mat, 3)[cola];
    //Normalize the plane.
    float t = sqrt(a*a+b*b+c*c);
    return PlaneT<float>(a/t, b/t, c/t, d/t);
  }

  static std::array<PlaneT<float>, 6> GetFrustum(const glm::mat4& mat)
  {
    std::array<PlaneT<float>, 6> frustPlanes;

    for(unsigned int i = 0; i < 6; ++i)
    {
      frustPlanes[i] = (GetClipPlane(mat, (ClipPlane)i));
    }
    return frustPlanes;
  }

  static glm::vec3 Vec3TransformH(const glm::mat4& mat, const glm::vec3& in) {
    glm::vec4 v =  mat * glm::vec4(in[0], in[1], in[2], 1.0f);
    return glm::vec3(v[0]/v[3], v[1]/v[3], v[2]/v[3]);
  }

};
}
#endif // MAT4EXT_H
