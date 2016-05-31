#include "shadowmapping.h"
#include "fpscamera.h"
#include <math/sphere.h>
#include <limits>
#include <QDebug>

namespace sa {
void ShadowMapping::updateShadowPass(const FPSCamera& camera, const FPSCamera& sunCamera) {
  m_depthBiasMVPMatrix.clear();
  m_shadowMapProjections.clear();

  for(int shadowPass = 0; shadowPass < getNumberOfPasses(); shadowPass++)
  {

    std::array<Vector3T<float>, 8> frustumpoints = camera.getFrusumPoints(m_cascadedProjections[shadowPass]);

    Matrix44T<float> shadowCamView = sunCamera.viewMatrix();

    Vector3T<float> min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vector3T<float> max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());


    for(int i = 0; i < 8; ++i) {
      Vector4T<float> vv = shadowCamView.Vec3Transform(frustumpoints[i]);
      for(int j = 0; j < 3; ++j) {
        min[j] = Min(vv[j], min[j]);
        max[j] = Max(vv[j], max[j]);
      }
    }

//    std::array<Vector3T<float>, 2> minmax = {min, max};
//    Sphere<float> sphere = Sphere<float>::createFromPoints<2>(minmax);
//    if(m_sphereradius[shadowPass] < -0.5)
//      m_sphereradius[shadowPass] = sphere.getRadius();

    // Calculate the view space extents of the frustum points.
//    float f = (m_sphereradius[shadowPass] * 2.0f) / float(m_shadowMapWidth);

//    Vector2T<float> thePos(floor(sphere.getPosition()[0]/f)*f, floor(sphere.getPosition()[1]/f)*f);
//    if(shadowPass == 0) {
//    //  qDebug() << "thePos " << thePos.X() << " " << thePos.Y();
//      qDebug() << f;
//    }
//    Vector2T<float> smin = thePos-m_sphereradius[shadowPass];
//    Vector2T<float> smax = thePos+m_sphereradius[shadowPass];

    Vector3T<float> smin = min;
    Vector3T<float> smax = max;


    //float viewportExtent = floor((m_sphereradius[shadowPass] * 2.0f) / f) * f;    // Ensure view point extents are a texel multiple.

    //smax[0] = smin[0] + viewportExtent;
    //smax[1] = smin[1] + viewportExtent;

    Matrix44T<float> ortho = Matrix44T<float>::GetOrthographicProjection(
          smin.X(), smax.X(), smin.Y(), smax.Y(), -max.Z(), -min.Z()
          );
//    Matrix44T<float> ortho = Matrix44T<float>::GetOrthographicProjection(
//          min.X(), max.X(), min.Y(), max.Y(), -max.Z(), -min.Z()
//          );

    static Matrix44T<float> biasMatrix(
          0.5, 0.0, 0.0, 0.5,
          0.0, 0.5, 0.0, 0.5,
          0.0, 0.0, 0.5, 0.5,
          0.0, 0.0, 0.0, 1.0);


    m_depthBiasMVPMatrix.push_back(biasMatrix * ortho * sunCamera.viewMatrix());
    m_shadowMapProjections.push_back(ortho);

  }
}

const std::vector<Matrix44T<float> >& ShadowMapping::getShadowMapProjections() const
{
  return m_shadowMapProjections;
}

const std::vector<Matrix44T<float> >& ShadowMapping::getDepthBiasMVPMatrix() const
{
  return m_depthBiasMVPMatrix;
}

unsigned int ShadowMapping::getShadowMapWidth() const
{
  return m_shadowMapWidth;
}

unsigned int ShadowMapping::getShadowMapHeight() const
{
  return m_shadowMapheight;
}
}
