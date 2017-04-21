#pragma once
#include <deque>
#include "serialization.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace sa {
class AnimationChannelModel
{
public:
  class TranslationKey
  {
  public:
    TranslationKey() {}
    TranslationKey(float time, const glm::vec3& t)
      : Time(time)
      , T(t){}

    float Time;
    glm::vec3 T;

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
    QuaternionKey(float time, const glm::quat& q)
      : Time(time)
      , Q(q) {}

    float Time;
    glm::quat Q;

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
