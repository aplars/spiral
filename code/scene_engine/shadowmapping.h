#ifndef SHADOWMAPPING_H
#define SHADOWMAPPING_H
#include <deque>
#include <vector>
#include <math/Matrix44T.h>

namespace sa {
class FPSCamera;

class ShadowMapping
{
public:
  ShadowMapping() {}

  void create(const std::vector<float>& shadowMapCascadeDist, float aspect, unsigned int shadowMapWidth, unsigned int shadowMapHeight)
  {
    float lastDist = 1;
    m_shadowMapCascadeDistance = shadowMapCascadeDist;
    for(float dist : m_shadowMapCascadeDistance)
    {
      m_cascadedProjections.push_back(sa::Matrix44T<float>::GetPerspectiveProjection(sa::DegToRad(60.0f), aspect, lastDist, dist));
      //lastDist = dist;
    }
    m_shadowMapWidth = shadowMapWidth;
    m_shadowMapheight = shadowMapHeight;

    for(int shadowPass = 0; shadowPass < m_shadowMapCascadeDistance.size(); shadowPass++)
    {
      m_sphereradius.push_back(-1);
    }
  }

  unsigned int getNumberOfPasses() const { return m_shadowMapCascadeDistance.size(); }
  const std::vector<float>& getShadowMapCascadeDistance() const { return m_shadowMapCascadeDistance; }

  void updateShadowPass(const FPSCamera& camera, const FPSCamera& sunCamera);


  const std::vector<Matrix44T<float> >& getShadowMapProjections() const;

  const std::vector<Matrix44T<float> >& getDepthBiasMVPMatrix() const;

  unsigned int getShadowMapWidth() const;

  unsigned int getShadowMapHeight() const;

private:
  std::vector<float> m_sphereradius;
  std::vector<float> m_shadowMapCascadeDistance;
  unsigned int m_shadowMapWidth = 0;
  unsigned int m_shadowMapheight = 0;
  std::vector<Matrix44T<float>> m_cascadedProjections;
  std::vector<Matrix44T<float>> m_shadowMapProjections;
  std::vector<Matrix44T<float>> m_depthBiasMVPMatrix;
};
}
#endif // SHADOWMAPPING_H
