#pragma once
#include <array>
#include "PlaneT.h"
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

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
  static Side FrustumAABBIntersect(const std::array<PlaneT<T>, 6>& planes, const glm::vec3& mins, const glm::vec3& maxs) {
    Side    ret = Inside;
    glm::vec3 vmin, vmax;

    for(int i = 0; i < 6; ++i) {
       // X axis
       if(planes[i].normal().x > 0) {
          vmin.x = (mins.x);
          vmax.y = (maxs.x);
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


       typename PlaneT<T>::Side sideA = planes[i].GetSide(vmin);
       typename PlaneT<T>::Side sideB = planes[i].GetSide(vmax);

       if(sideA != sideB)
         return Intersect;
       if(sideA == PlaneT<T>::Side::Back_Side && sideB == PlaneT<T>::Side::Back_Side) {
         return Outside;
       }

    }
    return ret;
  }


};
}
