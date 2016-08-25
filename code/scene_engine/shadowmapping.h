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

  void create(const std::vector<float>& shadowMapCascadeDist, float aspect, unsigned int shadowMapWidth, unsigned int shadowMapHeight);

  unsigned int getNumberOfPasses() const { return m_shadowMapCascadeDistance.size(); }
  const std::vector<float>& getShadowMapCascadeDistance() const { return m_shadowMapCascadeDistance; }

  void updateShadowPass(const FPSCamera& camera, const FPSCamera& sunCamera);

  bool isAABBVisibleFromSun(FPSCamera& sunCamera, const sa::Vector3T<float>& mins, const sa::Vector3T<float>& maxs) const;

  const std::vector<Matrix44T<float> >& getShadowMapProjections() const;

  const std::vector<Matrix44T<float> >& getDepthBiasMVPMatrix() const;

  unsigned int getShadowMapWidth() const;

  unsigned int getShadowMapHeight() const;

  bool getStable() const;
  void setStable(bool stable);

private:
  bool m_stable = false;
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
