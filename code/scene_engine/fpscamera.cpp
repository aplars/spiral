#include "fpscamera.h"
#include <math/Matrix44T.h>
#include <math/intersectiontests.h>
#include <limits>
#include <algorithm>

namespace  sa {
FPSCamera::~FPSCamera()
{

}

FPSCamera::FPSCamera()
{
  m_eye = glm::vec3(0.0, 0.0, 0.0);
  m_xAxis = glm::vec3(1.0, 0.0, 0.0);
  m_yAxis = glm::vec3(0.0, 1.0, 0.0);
  m_zAxis = glm::vec3(0.0, 0.0, 1.0);

  update(true);
}

void FPSCamera::setLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up) {
  m_eye = eye;
  m_zAxis = glm::normalize(eye - center);
  m_xAxis = glm::normalize(glm::cross(up, m_zAxis));
  m_yAxis = glm::normalize(glm::cross(m_zAxis, m_xAxis));
  update(false);
}

glm::vec3 FPSCamera::eye() const
{
  return m_eye;
}

void FPSCamera::setEye(const glm::vec3& eye)
{
  m_eye = eye;
  update(false);
}
Matrix44T<float> FPSCamera::viewMatrix() const
{
  return m_viewMatrix;
}

void FPSCamera::setViewMatrix(const Matrix44T<float> &viewMatrix)
{
  m_viewMatrix = viewMatrix;
}

void FPSCamera::moveForward(float amount) {
  m_eye = m_eye - m_zAxis*amount;
  update(false);
}

void FPSCamera::moveRight(float amount) {
  m_eye = m_eye - m_xAxis*amount;
  update(false);
}

void FPSCamera::rotate(const float& heading, const float& pitch, const float& roll)
{
  Matrix44T<float> rotMtx;
  glm::vec4 result;

  // Rotate camera's existing x and z axes about its existing y axis.
  if (heading != 0.0)
  {
    //rotMtx.LoadRotate(heading, m_yAxis);
    rotMtx.LoadRotate(heading, glm::vec3(0,1,0));

    result = rotMtx.Vec3Transform(m_xAxis);
    m_xAxis = glm::vec3(result);

    result = rotMtx.Vec3Transform(m_zAxis);
    m_zAxis = glm::vec3(result);
  }

  // Rotate camera's existing y and z axes about its existing x axis.
  if (pitch != 0.0)
  {
    rotMtx.LoadRotate(pitch, m_xAxis);

    result = rotMtx.Vec3Transform(m_yAxis);
    m_yAxis = glm::vec3(result[0], result[1], result[2]);

    result = rotMtx.Vec3Transform(m_zAxis);
    m_zAxis = glm::vec3(result[0], result[1], result[2]);
  }

  // Rotate camera's existing x and y axes about its existing z axis.
  if (roll != 0.0f)
  {
    rotMtx.LoadRotate(roll, m_zAxis);

    result = rotMtx.Vec3Transform(m_xAxis);
    m_xAxis = glm::vec3(result[0], result[1], result[2]);

    result = rotMtx.Vec3Transform(m_yAxis);
    m_yAxis = glm::vec3(result[0], result[1], result[2]);
  }

  update(true);
}

std::array<PlaneT<float>, 6> FPSCamera::getFrustum(const Matrix44T<float>& projection) const {
  return (projection*viewMatrix()).GetFrustum();
}

std::array<glm::vec3, 8> FPSCamera::getFrusumPoints(const Matrix44T<float>& projection) const {
  Matrix44T<float> viewProj = projection*viewMatrix();

  PlaneT<float> left = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Left);
  PlaneT<float> right = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Right);
  PlaneT<float> top = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Top);
  PlaneT<float> bottom = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Bottom);
  PlaneT<float> near = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Near);
  PlaneT<float> far = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Far);

  glm::vec3 p0 = IntersectionTests::ThreePlanesIntersectVec3(left, bottom, near);
  glm::vec3 p1 = IntersectionTests::ThreePlanesIntersectVec3(left, top, near);
  glm::vec3 p2 = IntersectionTests::ThreePlanesIntersectVec3(right, top, near);
  glm::vec3 p3 = IntersectionTests::ThreePlanesIntersectVec3(right, bottom, near);

  glm::vec3 p4 = IntersectionTests::ThreePlanesIntersectVec3(left, bottom, far);
  glm::vec3 p5 = IntersectionTests::ThreePlanesIntersectVec3(left, top, far);
  glm::vec3 p6 = IntersectionTests::ThreePlanesIntersectVec3(right, top, far);
  glm::vec3 p7 = IntersectionTests::ThreePlanesIntersectVec3(right, bottom, far);

  std::array<glm::vec3, 8> pnts;
  pnts[0] = p0;
  pnts[1] = p1;
  pnts[2] = p2;
  pnts[3] = p3;
  pnts[4] = p4;
  pnts[5] = p5;
  pnts[6] = p6;
  pnts[7] = p7;
  return pnts;
}

glm::vec3 FPSCamera::getFrusumCenterPoint(const Matrix44T<float>& projection) const {
  std::array<glm::vec3, 8> points = getFrusumPoints(projection);
  glm::vec3 fmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
  glm::vec3 fmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

  for(const glm::vec3& pnt : points) {
    fmin[0] = Min(pnt[0], fmin[0]);
    fmin[1] = Min(pnt[1], fmin[1]);
    fmin[2] = Min(pnt[2], fmin[2]);

    fmax[0] = Max(pnt[0], fmax[0]);
    fmax[1] = Max(pnt[1], fmax[1]);
    fmax[2] = Max(pnt[2], fmax[2]);
  }
  glm::vec3 halfDelta = (fmax-fmin)/2.0f;
  glm::vec3 center(fmin + halfDelta);

  return center;
}

void FPSCamera::update(bool orthogonalizeAxes)
{
  if (orthogonalizeAxes)
  {
    // Regenerate the camera's local axes to orthogonalize them.

    m_zAxis = glm::normalize(m_zAxis);

    m_yAxis = glm::cross(m_zAxis, m_xAxis);
    m_yAxis = glm::normalize(m_yAxis);

    m_xAxis = glm::cross(m_yAxis, m_zAxis);
    m_xAxis = glm::normalize(m_xAxis);
  }

  // Reconstruct the view matrix.

  m_viewMatrix[0][0] = m_xAxis[0];
  m_viewMatrix[1][0] = m_xAxis[1];
  m_viewMatrix[2][0] = m_xAxis[2];
  m_viewMatrix[3][0] = -glm::dot(m_xAxis, m_eye);

  m_viewMatrix[0][1] = m_yAxis[0];
  m_viewMatrix[1][1] = m_yAxis[1];
  m_viewMatrix[2][1] = m_yAxis[2];
  m_viewMatrix[3][1] = -glm::dot(m_yAxis, m_eye);

  m_viewMatrix[0][2] = m_zAxis[0];
  m_viewMatrix[1][2] = m_zAxis[1];
  m_viewMatrix[2][2] = m_zAxis[2];
  m_viewMatrix[3][2] = -glm::dot(m_zAxis, m_eye);

  m_viewMatrix[0][3] = 0.0f;
  m_viewMatrix[1][3] = 0.0f;
  m_viewMatrix[2][3] = 0.0f;
  m_viewMatrix[3][3] = 1.0f;
}

}
