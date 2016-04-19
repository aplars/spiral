#pragma once

#include <math/Vector3T.h>
#include "serialization.h"

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
  AABBModel(const sa::Vector3T<float>& center, const sa::Vector3T<float>& halfSize);

  const Vector3T<float>& getCenter() const { return m_center; }

  /**
   * @brief getHalfSize returns the size of the box.
   * @return the size of the box.
   */
  const Vector3T<float>& getHalfSize() const { return m_halfSize; }
  /**
   * @brief getMin - returns the minimum point in the box.
   * @return - the minimum
   */
  Vector3T<float> getMin() const { return m_center - m_halfSize; }
  /**
   * @brief getMax - returns the maximum point in the box.
   * @return - the maximum
   */
  Vector3T<float> getMax() const { return m_center + m_halfSize; }

  void expand(const AABBModel& other);
  void expand(const Vector3T<float>& e);
  void transform(const Matrix44T<float>& xform);
  /**
   * @brief createFromMinMax
   * @param min
   * @param max
   * @return
   */
  static AABBModel createFromMinMax(const sa::Vector3T<float>& min, const sa::Vector3T<float>& max);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int )
  {
    ar & BOOST_SERIALIZATION_NVP(m_center);
    ar & BOOST_SERIALIZATION_NVP(m_halfSize);
  }
  Vector3T<float> m_center;
  Vector3T<float> m_halfSize;
};
}
