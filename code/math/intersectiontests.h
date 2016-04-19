#pragma once
#include <array>
#include "PlaneT.h"

namespace sa {
class IntersectionTests {
public:
  enum Side {
    Inside,
    Outside,
    Intersect
  };

  template <typename T>
  static Vector3T<T> ThreePlanesIntersect(const PlaneT<T>& p1, const PlaneT<T>& p2, const PlaneT<T>& p3) {
    Vector3T<T> a =
        -p1.d() * (p2.normal().CrossProduct(p3.normal()))
        -p2.d() * (p3.normal().CrossProduct(p1.normal()))
        -p3.d() * (p1.normal().CrossProduct(p2.normal()));
    T b =
        p1.normal().DotProduct(p2.normal().CrossProduct(p3.normal()));

    return a/b;
  }

  template <typename T>
  static Side FrustumAABBIntersect(const std::array<PlaneT<T>, 6>& planes, const Vector3T<T>& mins, const Vector3T<T>& maxs) {
    Side    ret = Inside;
    Vector3T<T> vmin, vmax;

    for(int i = 0; i < 6; ++i) {
       // X axis
       if(planes[i].normal().X() > 0) {
          vmin.X(mins.X());
          vmax.X(maxs.X());
       } else {
          vmin.X(maxs.X());
          vmax.X(mins.X());
       }
       // Y axis
       if(planes[i].normal().Y() > 0) {
          vmin.Y(mins.Y());
          vmax.Y(maxs.Y());
       } else {
          vmin.Y(maxs.Y());
          vmax.Y(mins.Y());
       }
       // Z axis
       if(planes[i].normal().Z() > 0) {
          vmin.Z(mins.Z());
          vmax.Z(maxs.Z());
       } else {
          vmin.Z(maxs.Z());
          vmax.Z(mins.Z());
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
