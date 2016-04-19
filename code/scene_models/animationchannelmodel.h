#pragma once
#include <deque>
#include "serialization.h"
#include <math/Vector3T.h>
#include <math/QuaternionT.h>
namespace sa {
class AnimationChannelModel
{
public:
  class TranslationKey
  {
  public:
    TranslationKey() {}
    TranslationKey(float time, const Vector3T<float>& t)
      : Time(time)
      , T(t){}

    float Time;
    Vector3T<float> T;

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int )
    {
      ar & BOOST_SERIALIZATION_NVP(Time);
      ar & BOOST_SERIALIZATION_NVP(T);
    }
  };

  class QuaternionKey
  {
  public:
    QuaternionKey() {}
    QuaternionKey(float time, const QuaternionT<float>& q)
      : Time(time)
      , Q(q) {}

    float Time;
    QuaternionT<float> Q;

  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int )
    {
      ar & BOOST_SERIALIZATION_NVP(Time);
      ar & BOOST_SERIALIZATION_NVP(Q);
    }
  };
  std::deque<QuaternionKey> Quaternion;
  std::deque<TranslationKey> Translation;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int )
  {
    ar & BOOST_SERIALIZATION_NVP(Quaternion);
    ar & BOOST_SERIALIZATION_NVP(Translation);
  }
};
}
