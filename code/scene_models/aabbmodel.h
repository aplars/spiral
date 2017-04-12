#pragma once

#include "serialization.h"
#include <glm/gtc/type_ptr.hpp>

namespace sa {
class AABBModel
{
public:
  AABBModel() {}
  /**
   * @brief AABB - creates an axis aligned bounding box from a center point and a half size.
   * @param center - the center of the box.
   * @param halfSize - the half size of the box.
   */
  AABBModel(const glm::vec3 &center, const glm::vec3 &halfSize);

  const glm::vec3& getCenter() const { return m_center; }

  /**
   * @brief getHalfSize returns the size of the box.
   * @return the size of the box.
   */
  const glm::vec3& getHalfSize() const { return m_halfSize; }
  /**
   * @brief getMin - returns the minimum point in the box.
   * @return - the minimum
   */
  glm::vec3 getMin() const { return (m_center - m_halfSize); }
  /**
   * @brief getMax - returns the maximum point in the box.
   * @return - the maximum
   */
  glm::vec3 getMax() const { return (m_center + m_halfSize); }

  void expand(const AABBModel& other);
  void expand(const glm::vec3& e);
  void transform(const glm::mat4 &xform);
  /**
   * @brief createFromMinMax
   * @param min
   * @param max
   * @return
   */
  static AABBModel createFromMinMax(const glm::vec3 &min, const glm::vec3 &max);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int )
  {
    ar & BOOST_SERIALIZATION_NVP(m_center);
    ar & BOOST_SERIALIZATION_NVP(m_halfSize);
  }
  glm::vec3 m_center;
  glm::vec3 m_halfSize;
};
}
