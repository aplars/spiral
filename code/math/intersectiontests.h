#pragma once
#include <array>
#include <deque>
#include "PlaneT.h"
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QDebug>

namespace sa {
class IntersectionTests {
public:
  enum Side {
    Inside,
    Outside,
    Intersect
  };

  template <typename T>
  static glm::vec3 ThreePlanesIntersectVec3(const PlaneT<T>& p1, const PlaneT<T>& p2, const PlaneT<T>& p3) {
    glm::vec3 a =
        -p1.d() * glm::cross(p2.normal(), p3.normal())
        -p2.d() * glm::cross(p3.normal(), p1.normal())
        -p3.d() * glm::cross(p1.normal(), p2.normal());

    T b =
        glm::dot(p1.normal(), glm::cross(p2.normal(), p3.normal()));

    return a/b;
  }

  template <typename T>
  static Side FrustumOBBIntersect(const std::array<PlaneT<T>, 6>& planes, const glm::vec3& pos, const glm::vec3& halfSize, const std::array<glm::vec3, 3>& axes) {
    int numInside = 0;
    for(const PlaneT<T>& plane : planes)
    {
      // Find the negative and positive far points of the OBB to the current frustum plane.
      float x = glm::dot(axes[0], plane.normal()) >= 0.f ? halfSize[0] : -halfSize[0];
      float y = glm::dot(axes[1], plane.normal()) >= 0.f ? halfSize[1] : -halfSize[1];
      float z = glm::dot(axes[2], plane.normal()) >= 0.f ? halfSize[2] : -halfSize[2];

      // There are eight half-diagonal vectors on an OBB. (A half-diagonal is a vector from OBB center to one of its
      // corner vertices). Compute the half-diagonal vector that points most in the same direction than the plane normal.
      const glm::vec3 diag = x*axes[0] + y*axes[1] + z*axes[2];

      // nPoint is now the corner point of the OBB that lies "the most" inside the frustum,
      // its projection on the plane normal extends the most to the negative direction of that normal.
      const glm::vec3 nPoint = pos - diag;

      const glm::vec3 pPoint = pos + diag;



      if (glm::dot(nPoint, plane.normal()) + plane.d() >= 0.f)
        return Side::Outside; // OBB outside frustum.

      // If we would like to check whether the OBB is fully inside the frustum, need to compute

      // Dot(pPoint, frustum.planes[i].normal) + frustum.planes[i].d. If it's < 0 for all planes, OBB is totally
      // inside the frustum and doesn't intersect any of the frustum planes.
      if((glm::dot(pPoint, plane.normal()) + plane.d()) < 0.0f) {
        ++numInside;
      }
    }
    if(numInside == 6) return Side::Inside;
    return Side::Intersect; // OBB inside the frustum or part of the OBB intersects the frustum.  }
  }

  template <typename T>
  static Side FrustumAABBIntersect(const std::array<PlaneT<T>, 6>& planes, const glm::vec3& mins, const glm::vec3& maxs) {
    std::deque<PlaneT<T>> planesDeque;
    std::copy(std::begin(planes), std::end(planes), std::back_inserter(planesDeque));
    return FrustumAABBIntersect(planesDeque, mins, maxs);
  }

  template <typename T>
  static Side FrustumAABBIntersect(const std::deque<PlaneT<T>>& planes, const glm::vec3& mins, const glm::vec3& maxs) {
    Side    ret = Inside;
    glm::vec3 vmin, vmax;

    vmin = mins;
    vmax = maxs;
    for(int i = 0; i < planes.size(); ++i) {
      // X axis
      if(planes[i].normal().x > 0) {
        vmin.x = (mins.x);
        vmax.x = (maxs.x);
      } else {
        vmin.x = (maxs.x);
        vmax.x = (mins.x);
      }
      // Y axis
      if(planes[i].normal().y > 0) {
        vmin.y = (mins.y);
        vmax.y = (maxs.y);
      } else {
        vmin.y = (maxs.y);
        vmax.y = (mins.y);
      }
      // Z axis
      if(planes[i].normal().z > 0) {
        vmin.z = (mins.z);
        vmax.z = (maxs.z);
      } else {
        vmin.z = (maxs.z);
        vmax.z = (mins.z);
      }

      if(glm::dot(planes[i].normal(), vmax) + planes[i].d() < 0)
        return Outside;
      if(glm::dot(planes[i].normal(), vmin) + planes[i].d() <= 0)
        ret = Intersect;
    }
    return ret;
  }

};
}
