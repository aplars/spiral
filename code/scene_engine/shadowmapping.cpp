#include "shadowmapping.h"
#include "fpscamera.h"
#include <math/sphereT.h>
#include <math/intersectiontests.h>
#include <math/vec3ext.h>
#include <limits>
#include <QDebug>
#include <math/mat4ext.h>
#include <glm/gtc/matrix_transform.hpp>

namespace sa {
void ShadowMapping::create(const std::vector<float> &shadowMapCascadeDist, float aspect, unsigned int shadowMapWidth, unsigned int shadowMapHeight)
{
  float lastDist = 1;
  m_shadowMapCascadeDistance = shadowMapCascadeDist;
  for(float dist : m_shadowMapCascadeDistance)
  {
    m_cascadedProjections.push_back(glm::perspective(sa::DegToRad(60.0f), aspect, lastDist, dist));
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

    std::array<glm::vec3, 8> frustumpoints = camera.getFrusumPoints(m_cascadedProjections[shadowPass]);
    //Sphere<float> tb = Sphere<float>::createFromPoints<8>(frustumpoints);


    glm::mat4 sunCameraViewMatrix = sunCamera.viewMatrix();

    glm::vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::vec3 max(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());


    for(int i = 0; i < 8; ++i) {
      glm::vec3 vv = Mat4ext::Vec3TransformH(sunCameraViewMatrix, frustumpoints[i]);
      min = Vec3ext::MinVec3(glm::vec3(vv.x, vv.y, vv.z), min);
      max = Vec3ext::MaxVec3(glm::vec3(vv.x, vv.y, vv.z), max);
    }

    glm::mat4 ortho;

//    if(m_stable) {
//      std::array<glm::vec3, 2> minmax = std::array<glm::vec3, 2>({min, max});
//      Sphere<float> sphere = Sphere<float>::createFromPoints<2>(minmax);


//      if(m_sphereradius[shadowPass] < -0.5)
//        m_sphereradius[shadowPass] = sphere.getRadius();

//      // Calculate the view space extents of the frustum points.
//      float f = (m_sphereradius[shadowPass] * 2.0f) / float(m_shadowMapWidth);

//      glm::vec3 thePos(floor(sphere.getPosition()[0]/f)*f, floor(sphere.getPosition()[1]/f)*f, floor(sphere.getPosition()[2]/f)*f);
//      glm::vec3 smin = thePos-m_sphereradius[shadowPass];
//      glm::vec3 smax = thePos+m_sphereradius[shadowPass];


//      float viewportExtent = floor((m_sphereradius[shadowPass] * 2.0f) / f) * f;    // Ensure view point extents are a texel multiple.

//      smax[0] = smin[0] + viewportExtent;
//      smax[1] = smin[1] + viewportExtent;
//      smax[2] = smin[2] + viewportExtent;

//      ortho = glm::ortho(smin.x, smax.x, smin.y, smax.y, -smax.z, -smin.z);
//    }
//    else {
      ortho = glm::ortho(min.x, max.x, min.y, max.y, -max.z, -min.z);
//    }

    static float biasMatrix[16] = {
          0.5, 0.0, 0.0, 0.0,
          0.0, 0.5, 0.0, 0.0,
          0.0, 0.0, 0.5, 0.0,
          0.5, 0.5, 0.5, 1.0 };


    m_depthBiasMVPMatrix.push_back(glm::make_mat4(biasMatrix) * ortho * sunCamera.viewMatrix());
    m_shadowMapProjections.push_back(ortho);

  }
}

bool ShadowMapping::isAABBVisibleFromSun(FPSCamera &sunCamera, const glm::vec3 &mins, const glm::vec3 &maxs) const
{
  std::array<PlaneT<float>, 6> frustum;

  for(const glm::mat4& shadowMapProjection : m_shadowMapProjections) {
     frustum = sunCamera.getFrustum(shadowMapProjection);
     std::deque<PlaneT<float>> frustumdeque;

     //We do not use the front plane here. Thats because it may be in fron of
     //objects that contributes to the scene.
     frustumdeque.push_back(frustum.at(sa::Mat4ext::ClipPlane::Left));
     frustumdeque.push_back(frustum.at(sa::Mat4ext::ClipPlane::Right));
     frustumdeque.push_back(frustum.at(sa::Mat4ext::ClipPlane::Bottom));
     frustumdeque.push_back(frustum.at(sa::Mat4ext::ClipPlane::Top));
     frustumdeque.push_back(frustum.at(sa::Mat4ext::ClipPlane::Far));

     sa::IntersectionTests::Side side = sa::IntersectionTests::FrustumAABBIntersect(frustumdeque, mins, maxs);
     if(side == sa::IntersectionTests::Side::Inside || side == sa::IntersectionTests::Side::Intersect) {
       return true;
     }
  }
  return false;
}

const std::vector<glm::mat4> &ShadowMapping::getShadowMapProjections() const
{
  return m_shadowMapProjections;
}

const std::vector<glm::mat4>& ShadowMapping::getDepthBiasMVPMatrix() const
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
