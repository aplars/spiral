#pragma once
#include "animationchannelmodel.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/map.hpp>
#pragma GCC diagnostic pop
#include <string>
#include <map>
#include <glm/gtx/compatibility.hpp>
#include "math/mat4ext.h"

namespace sa {
template <typename ChannelKey>
class AnimationModel
{
public:
  /**
   * @brief AnimationChannels The key frames. Arg0 is the name of the skeleton's joint. Arg1 is the key frame. Maps channels to joints.
   */
  typedef std::map<ChannelKey, AnimationChannelModel> AnimationChannels;

  AnimationModel() {}
  ~AnimationModel() {}

  ///Returns the transformation at time on joint.
  bool getTransformation(const float time, const ChannelKey& joint, glm::mat4& outMatrix) const;

  std::string Name;
  float Duration;
  AnimationChannels Channels;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int ) {
    ar & BOOST_SERIALIZATION_NVP(Name);
    ar & BOOST_SERIALIZATION_NVP(Duration);
    ar & BOOST_SERIALIZATION_NVP(Channels);
  }
};

template <typename ChannelKey>
bool AnimationModel<ChannelKey>::getTransformation(const float time, const ChannelKey& joint, glm::mat4& outMatrix) const {
  //Get the channel used for updating the joint.
  typename AnimationChannels::const_iterator it = Channels.find(joint);
  if(it == Channels.end())
    return false;
  AnimationChannelModel channel = it->second;

  //Find the frame at the current time.
  unsigned int i = 0;
  while(i < channel.Translation.size()-1)
  {
    if(channel.Translation[i].Time >= time)
      break;
    i++;
  }

  glm::vec3 translate = channel.Translation[i].T;

  if(i > 0)
  {
    //Interpolate between last and current key frame.
    float t = (time-(channel.Translation[i-1].Time))/(channel.Translation[i].Time-channel.Translation[i-1].Time);
    translate = glm::lerp(channel.Translation[i-1].T, channel.Translation[i].T, t);
  }

  //Find the frame at the current time.
  i = 0;
  while(i < channel.Quaternion.size()-1)
  {
    if(channel.Quaternion[i].Time >= time)
      break;
    i++;
  }

  glm::mat4 Qm;

  if(i > 0)
  {
    //Interpolate between last and current key frame.
    float t = (time-channel.Quaternion[i-1].Time)/(channel.Quaternion[i].Time-channel.Quaternion[i-1].Time);
    sa::QuaternionT<float> Q = sa::QuaternionT<float>::SLerp(t, channel.Quaternion[i-1].Q, channel.Quaternion[i].Q);
    Qm =(Q.GetMat4());
  }
  else
  {
    Qm = (channel.Quaternion[i].Q.GetMat4());
  }


  Qm[3][0] = translate[0];
  Qm[3][1] = translate[1];
  Qm[3][2] = translate[2];


  //outMatrix = Mat4ext::toMat4(Qm);
  outMatrix = (Qm);
  return true;
}

}

