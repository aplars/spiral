#include "aabbmodel.h"
#include <algorithm>

namespace sa {
AABBModel::AABBModel(const glm::vec3& center, const glm::vec3& halfSize)
  : m_center(center)
  , m_halfSize(halfSize)
{ }

void AABBModel::expand(const AABBModel& other) {
  expand(other.getMin());
  expand(other.getMax());
}

void AABBModel::expand(const glm::vec3 &e) {
  glm::vec3 _min = getMin();
  glm::vec3 _max = getMax();

  for(unsigned int i = 0; i < 3; ++i) {
    _min[i] = std::min(_min[i], e[i]);
    _max[i] = std::max(_max[i], e[i]);
  }
  *this = createFromMinMax(_min, _max);
}

void AABBModel::transform(const Matrix44T<float>& xform) {
  glm::vec3 _min = getMin();
  glm::vec3 _max = getMax();

  *this = createFromMinMax(xform.Vec3TransformH(_min), xform.Vec3TransformH(_max));

  expand(          xform.Vec3TransformH(glm::vec3(_min[0], _min[1], _max[2])));
  expand(          xform.Vec3TransformH(glm::vec3(_min[0], _max[1], _min[2])));
  expand(          xform.Vec3TransformH(glm::vec3(_min[0], _max[1], _max[2])));
  expand(          xform.Vec3TransformH(glm::vec3(_max[0], _min[1], _min[2])));
  expand(          xform.Vec3TransformH(glm::vec3(_max[0], _min[1], _max[2])));
  expand(          xform.Vec3TransformH(glm::vec3(_max[0], _max[1], _min[2])));
  expand(          xform.Vec3TransformH(glm::vec3(_max[0], _max[1], _max[2])));

}

/*static*/ AABBModel AABBModel::createFromMinMax(const glm::vec3& min, const glm::vec3& max) {
  glm::vec3 halfSize((max-min)/2.0f);
  glm::vec3 center(min+halfSize);
  return AABBModel(center, halfSize);
}
}
