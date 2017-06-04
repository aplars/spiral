#ifndef SHADOWMAPPING_H
#define SHADOWMAPPING_H
#include <deque>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
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

  bool isAABBVisibleFromSun(FPSCamera& sunCamera, const glm::vec3& mins, const glm::vec3& maxs) const;

  const std::vector<glm::mat4>& getShadowMapProjections() const;

  const std::vector<glm::mat4>& getDepthBiasMVPMatrix() const;

  unsigned int getShadowMapWidth() const;

  unsigned int getShadowMapHeight() const;

private:
  bool m_stable = false;
  std::vector<float> m_sphereradius;
  std::vector<float> m_shadowMapCascadeDistance;
  unsigned int m_shadowMapWidth = 0;
  unsigned int m_shadowMapheight = 0;
  std::vector<glm::mat4> m_cascadedProjections;
  std::vector<glm::mat4> m_shadowMapProjections;
  std::vector<glm::mat4> m_depthBiasMVPMatrix;
};
}
#endif // SHADOWMAPPING_H
