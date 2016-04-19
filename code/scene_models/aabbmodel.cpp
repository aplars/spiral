#include "aabbmodel.h"
#include <algorithm>

namespace sa {
AABBModel::AABBModel(const sa::Vector3T<float>& center, const sa::Vector3T<float>& halfSize)
  : m_center(center)
  , m_halfSize(halfSize)
{ }

void AABBModel::expand(const AABBModel& other) {
  expand(other.getMin());
  expand(other.getMax());
}

void AABBModel::expand(const Vector3T<float>& e) {
  Vector3T<float> _min = getMin();
  Vector3T<float> _max = getMax();

  for(unsigned int i = 0; i < 3; ++i) {
    _min[i] = std::min(_min[i], e[i]);
    _max[i] = std::max(_max[i], e[i]);
  }
  *this = createFromMinMax(_min, _max);
}

void AABBModel::transform(const Matrix44T<float>& xform) {
  Vector3T<float> _min = getMin();
  Vector3T<float> _max = getMax();

  *this = createFromMinMax(xform.Vec3Transform(_min[0], _min[1], _min[2]).XYZ(), xform.Vec3Transform(_min[0], _min[1], _min[2]).XYZ());

  expand(          xform.Vec3Transform(_min[0], _min[1], _max[2]).XYZ());
  expand(          xform.Vec3Transform(_min[0], _max[1], _min[2]).XYZ());
  expand(          xform.Vec3Transform(_min[0], _max[1], _max[2]).XYZ());
  expand(          xform.Vec3Transform(_max[0], _min[1], _min[2]).XYZ());
  expand(          xform.Vec3Transform(_max[0], _min[1], _max[2]).XYZ());
  expand(          xform.Vec3Transform(_max[0], _max[1], _min[2]).XYZ());
  expand(          xform.Vec3Transform(_max[0], _max[1], _max[2]).XYZ());

}

/*static*/ AABBModel AABBModel::createFromMinMax(const sa::Vector3T<float>& min, const sa::Vector3T<float>& max) {
  sa::Vector3T<float> halfSize((max-min)/2.0f);
  sa::Vector3T<float> center(min+halfSize);
  return AABBModel(center, halfSize);
}
}
