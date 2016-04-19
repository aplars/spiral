#ifndef FPSCAMERA_H
#define FPSCAMERA_H
#include <math/Vector3T.h>
#include <math/Matrix44T.h>
#include <math/FrustumT.h>
namespace sa {
class FPSCamera
{
public:
  ~FPSCamera();
  FPSCamera();

  void setLookAt(const Vector3T<float>& eye, const Vector3T<float>& center, const Vector3T<float>& up);

  Vector3T<float> eye() const;
  void setEye(const Vector3T<float> &eye);

  Matrix44T<float> viewMatrix() const;
  void setViewMatrix(const Matrix44T<float> &viewMatrix);
  void moveForward(float amount);
  void moveRight(float amount);
  void rotate(const float& heading, const float& pitch, const float& roll);
  std::array<PlaneT<float>, 6> getFrustum(const Matrix44T<float>& projection) const;
  std::array<Vector3T<float>, 8> getFrusumPoints(const Matrix44T<float>& projection) const;
  Vector3T<float> getFrusumCenterPoint(const Matrix44T<float>& projection) const;
private:
  void update(bool orthogonalizeAxes);

  Matrix44T<float> m_viewMatrix;
  Vector3T<float> m_eye;
  Vector3T<float> m_xAxis;
  Vector3T<float> m_yAxis;
  Vector3T<float> m_zAxis;
};
}
#endif // FPSCAMERA_H
