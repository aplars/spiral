#pragma once

#include <math/FrustumT.h>
#define GLM_SWIZZLE
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <memory>

namespace sa {
class FPSCamera
{
public:
  ~FPSCamera();
  FPSCamera();

  void setLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up);

  glm::vec3 eye() const;
  void setEye(const glm::vec3 &eye);

  glm::mat4 viewMatrix() const;
  void setViewMatrix(const glm::mat4 &viewMatrix);
  void moveForward(float amount);
  void moveRight(float amount);
  void moveUp(float amount);
  void rotate(const float& heading, const float& pitch, const float& roll);
  std::array<PlaneT<float>, 6> getFrustum(const glm::mat4& projection) const;
  std::array<glm::vec3, 8> getFrusumPoints(const glm::mat4& projection) const;
  glm::vec3 getFrusumCenterPoint(const glm::mat4& projection) const;
private:
  void update(bool orthogonalizeAxes);

  glm::mat4 m_viewMatrix;
  glm::vec3 m_eye;
  glm::vec3 m_xAxis;
  glm::vec3 m_yAxis;
  glm::vec3 m_zAxis;
};
typedef std::shared_ptr<FPSCamera> FPSCameraPtr;
}
