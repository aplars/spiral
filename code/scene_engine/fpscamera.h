#ifndef FPSCAMERA_H
#define FPSCAMERA_H
#include <math/Matrix44T.h>
#include <math/FrustumT.h>
#include <glm/vec3.hpp>

namespace sa {
class FPSCamera
{
public:
  ~FPSCamera();
  FPSCamera();

  void setLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

  glm::vec3 eye() const;
  void setEye(const glm::vec3 &eye);

  Matrix44T<float> viewMatrix() const;
  void setViewMatrix(const Matrix44T<float> &viewMatrix);
  void moveForward(float amount);
  void moveRight(float amount);
  void rotate(const float& heading, const float& pitch, const float& roll);
  std::array<PlaneT<float>, 6> getFrustum(const Matrix44T<float>& projection) const;
  std::array<glm::vec3, 8> getFrusumPoints(const Matrix44T<float>& projection) const;
  glm::vec3 getFrusumCenterPoint(const Matrix44T<float>& projection) const;
private:
  void update(bool orthogonalizeAxes);

  Matrix44T<float> m_viewMatrix;
  glm::vec3 m_eye;
  glm::vec3 m_xAxis;
  glm::vec3 m_yAxis;
  glm::vec3 m_zAxis;
};
}
#endif // FPSCAMERA_H
