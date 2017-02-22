#include "shadowmapping.h"
#include "fpscamera.h"
#include <math/sphere.h>
#include <math/intersectiontests.h>
#include <math/vec3ext.h>
#include <limits>
#include <QDebug>
#include <math/mat4ext.h>

namespace sa {
void ShadowMapping::create(const std::vector<float> &shadowMapCascadeDist, float aspect, unsigned int shadowMapWidth, unsigned int shadowMapHeight)
{
  float lastDist = 1;
  m_shadowMapCascadeDistance = shadowMapCascadeDist;
  for(float dist : m_shadowMapCascadeDistance)
  {
    m_cascadedProjections.push_back(sa::Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0f), aspect, lastDist, dist));
    lastDist = dist;
  }
  m_shadowMapWidth = shadowMapWidth;
  m_shadowMapheight = shadowMapHeight;

  for(unsigned int shadowPass = 0; shadowPass < m_shadowMapCascadeDistance.size(); shadowPass++)
  {
    m_sphereradius.push_back(-1);
  }
}

void ShadowMapping::updateShadowPass(const FPSCamera& camera, const FPSCamera& sunCamera) {
  m_depthBiasMVPMatrix.clear();
  m_shadowMapProjections.clear();

  for(unsigned int shadowPass = 0; shadowPass < getNumberOfPasses(); shadowPass++)
  {

    std::array<glm::vec3, 8> frustumpoints = camera.getFrusumPoints(Mat4ext::toMat4(m_cascadedProjections[shadowPass]));
    //Sphere<float> tb = Sphere<float>::createFromPoints<8>(frustumpoints);


    Matrix44T<float> sunCameraViewMatrix = Mat4ext::fromMat4(sunCamera.viewMatrix());

    glm::vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::vec3 max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());


    for(int i = 0; i < 8; ++i) {
      glm::vec4 vv = sunCameraViewMatrix.Vec3Transform(frustumpoints[i]);
      min = Vec3ext::MinVec3(glm::vec3(vv.x, vv.y, vv.z), min);
      max = Vec3ext::MaxVec3(glm::vec3(vv.x, vv.y, vv.z), max);
    }

    Matrix44T<float> ortho;

    if(m_stable) {
      std::array<glm::vec3, 2> minmax = std::array<glm::vec3, 2>({min, max});
      Sphere<float> sphere = Sphere<float>::createFromPoints<2>(minmax);


      if(m_sphereradius[shadowPass] < -0.5)
        m_sphereradius[shadowPass] = sphere.getRadius();

      // Calculate the view space extents of the frustum points.
      float f = (m_sphereradius[shadowPass] * 2.0f) / float(m_shadowMapWidth);

      glm::vec3 thePos(floor(sphere.getPosition()[0]/f)*f, floor(sphere.getPosition()[1]/f)*f, floor(sphere.getPosition()[2]/f)*f);
      glm::vec3 smin = thePos-m_sphereradius[shadowPass];
      glm::vec3 smax = thePos+m_sphereradius[shadowPass];


      float viewportExtent = floor((m_sphereradius[shadowPass] * 2.0f) / f) * f;    // Ensure view point extents are a texel multiple.

      smax[0] = smin[0] + viewportExtent;
      smax[1] = smin[1] + viewportExtent;
      smax[2] = smin[2] + viewportExtent;

      ortho = Matrix44T<float>::GetOrthographicProjection(
            smin.x, smax.x, smin.y, smax.y, -smax.z, -smin.z
            );
    }
    else {
      ortho = Matrix44T<float>::GetOrthographicProjection(
            min.x, max.x, min.y, max.y, -max.z, -min.z
            );
    }
    static Matrix44T<float> biasMatrix(
          0.5, 0.0, 0.0, 0.5,
          0.0, 0.5, 0.0, 0.5,
          0.0, 0.0, 0.5, 0.5,
          0.0, 0.0, 0.0, 1.0);


    m_depthBiasMVPMatrix.push_back(biasMatrix * ortho * Mat4ext::fromMat4(sunCamera.viewMatrix()));
    m_shadowMapProjections.push_back(ortho);

  }
}

bool ShadowMapping::isAABBVisibleFromSun(FPSCamera &sunCamera, const glm::vec3 &mins, const glm::vec3 &maxs) const
{
  for(const Matrix44T<float>& shadowMapProjection : m_shadowMapProjections) {
     std::array<PlaneT<float>, 6> frustum = sunCamera.getFrustum(Mat4ext::toMat4(shadowMapProjection));
     sa::IntersectionTests::Side side = sa::IntersectionTests::FrustumAABBIntersect(frustum, mins, maxs);
     if(side == sa::IntersectionTests::Side::Inside || side == sa::IntersectionTests::Side::Intersect) {
       return true;
     }
  }
  return false;
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

bool ShadowMapping::getStable() const
{
  return m_stable;
}

void ShadowMapping::setStable(bool stable)
{
  m_stable = stable;
}
}
