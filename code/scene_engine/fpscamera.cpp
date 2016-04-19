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
  m_eye.Set(0.0, 0.0, 0.0);
  m_xAxis.Set(1.0, 0.0, 0.0);
  m_yAxis.Set(0.0, 1.0, 0.0);
  m_zAxis.Set(0.0, 0.0, 1.0);

  update(true);
}

void FPSCamera::setLookAt(const Vector3T<float>& eye, const Vector3T<float>& center, const Vector3T<float>& up) {
  m_eye = eye;
  m_zAxis = (eye - center).GetNormalized();
  m_xAxis = (up.CrossProduct(m_zAxis)).GetNormalized();
  m_yAxis = m_zAxis.CrossProduct(m_xAxis).GetNormalized();
  update(false);
}

Vector3T<float> FPSCamera::eye() const
{
  return m_eye;
}

void FPSCamera::setEye(const Vector3T<float> &eye)
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
  Vector4T<float> result;

  // Rotate camera's existing x and z axes about its existing y axis.
  if (heading != 0.0)
  {
    //rotMtx.LoadRotate(heading, m_yAxis);
    rotMtx.LoadRotate(heading, Vector3T<float>(0,1,0));

    result = rotMtx.Vec3Transform(m_xAxis);
    m_xAxis = Vector3T<float>(result[0], result[1], result[2]);

    result = rotMtx.Vec3Transform(m_zAxis);
    m_zAxis = Vector3T<float>(result[0], result[1], result[2]);
  }

  // Rotate camera's existing y and z axes about its existing x axis.
  if (pitch != 0.0)
  {
    rotMtx.LoadRotate(pitch, m_xAxis);

    result = rotMtx.Vec3Transform(m_yAxis);
    m_yAxis = Vector3T<float>(result[0], result[1], result[2]);

    result = rotMtx.Vec3Transform(m_zAxis);
    m_zAxis = Vector3T<float>(result[0], result[1], result[2]);
  }

  // Rotate camera's existing x and y axes about its existing z axis.
  if (roll != 0.0f)
  {
    rotMtx.LoadRotate(roll, m_zAxis);

    result = rotMtx.Vec3Transform(m_xAxis);
    m_xAxis = Vector3T<float>(result[0], result[1], result[2]);

    result = rotMtx.Vec3Transform(m_yAxis);
    m_yAxis = Vector3T<float>(result[0], result[1], result[2]);
  }

  update(true);
}

std::array<PlaneT<float>, 6> FPSCamera::getFrustum(const Matrix44T<float>& projection) const {
  return (projection*viewMatrix()).GetFrustum();
}

std::array<Vector3T<float>, 8> FPSCamera::getFrusumPoints(const Matrix44T<float>& projection) const {
  Matrix44T<float> viewProj = projection*viewMatrix();

  PlaneT<float> left = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Left);
  PlaneT<float> right = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Right);
  PlaneT<float> top = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Top);
  PlaneT<float> bottom = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Bottom);
  PlaneT<float> near = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Near);
  PlaneT<float> far = viewProj.GetClipPlane(Matrix44T<float>::ClipPlane::Far);

  Vector3T<float> p0 = IntersectionTests::ThreePlanesIntersect(left, bottom, near);
  Vector3T<float> p1 = IntersectionTests::ThreePlanesIntersect(left, top, near);
  Vector3T<float> p2 = IntersectionTests::ThreePlanesIntersect(right, top, near);
  Vector3T<float> p3 = IntersectionTests::ThreePlanesIntersect(right, bottom, near);

  Vector3T<float> p4 = IntersectionTests::ThreePlanesIntersect(left, bottom, far);
  Vector3T<float> p5 = IntersectionTests::ThreePlanesIntersect(left, top, far);
  Vector3T<float> p6 = IntersectionTests::ThreePlanesIntersect(right, top, far);
  Vector3T<float> p7 = IntersectionTests::ThreePlanesIntersect(right, bottom, far);

  std::array<Vector3T<float>, 8> pnts;
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

Vector3T<float> FPSCamera::getFrusumCenterPoint(const Matrix44T<float>& projection) const {
  std::array<Vector3T<float>, 8> points = getFrusumPoints(projection);
  Vector3T<float> fmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
  Vector3T<float> fmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());

  for(const Vector3T<float>& pnt : points) {
    fmin[0] = Min(pnt[0], fmin[0]);
    fmin[1] = Min(pnt[1], fmin[1]);
    fmin[2] = Min(pnt[2], fmin[2]);

    fmax[0] = Max(pnt[0], fmax[0]);
    fmax[1] = Max(pnt[1], fmax[1]);
    fmax[2] = Max(pnt[2], fmax[2]);
  }
  Vector3T<float> halfDelta = (fmax-fmin)/2.0f;
  Vector3T<float> center(fmin + halfDelta);

  return center;
}

void FPSCamera::update(bool orthogonalizeAxes)
{
  if (orthogonalizeAxes)
  {
    // Regenerate the camera's local axes to orthogonalize them.

    m_zAxis.Normalize();

    m_yAxis = m_zAxis.CrossProduct(m_xAxis);
    m_yAxis.Normalize();

    m_xAxis = m_yAxis.CrossProduct(m_zAxis);
    m_xAxis.Normalize();
  }

  // Reconstruct the view matrix.

  m_viewMatrix[0][0] = m_xAxis[0];
  m_viewMatrix[1][0] = m_xAxis[1];
  m_viewMatrix[2][0] = m_xAxis[2];
  m_viewMatrix[3][0] = -m_xAxis.DotProduct(m_eye);

  m_viewMatrix[0][1] = m_yAxis[0];
  m_viewMatrix[1][1] = m_yAxis[1];
  m_viewMatrix[2][1] = m_yAxis[2];
  m_viewMatrix[3][1] = -m_yAxis.DotProduct(m_eye);

  m_viewMatrix[0][2] = m_zAxis[0];
  m_viewMatrix[1][2] = m_zAxis[1];
  m_viewMatrix[2][2] = m_zAxis[2];
  m_viewMatrix[3][2] = -m_zAxis.DotProduct(m_eye);

  m_viewMatrix[0][3] = 0.0f;
  m_viewMatrix[1][3] = 0.0f;
  m_viewMatrix[2][3] = 0.0f;
  m_viewMatrix[3][3] = 1.0f;
}

}
