#include "skeletonmodel.h"
#include <algorithm>
#include <list>

namespace sa
{
	Skeleton::Skeleton()
	{ }

  void Skeleton::reset() {
    for(const std::string name : getAnimationNames()) {
      animate(0, name);
    }
  }

  void Skeleton::animate(float currentTime, const std::string& animName)
  {
    float durationTime = Animations[animName].Duration;

    float currentTimeRepeated = fmod(currentTime, durationTime);

    //Update all joints
    for(AnimationModel<std::string >::AnimationChannels::value_type channel : Animations[animName].Channels)
    {
      const std::string tmp = channel.first;
      //Apply transformation on joint.
      Animations[animName].getTransformation(currentTimeRepeated, channel.first, Joints[tmp].LocalTransformation);
    }
  }

  void Skeleton::applyTransformations() {

    std::list<std::string> parentStack;
    std::list<std::string> traverser;
    std::map<std::string, Matrix44T<float> > absoluteTransforms;
    traverser.push_back(Root);
    //Traverse the joint hierarchy and store the absolute transforms.
    while(!traverser.empty())
    {
      std::string cur = traverser.front();
      std::string parent = "-1";

      if(!parentStack.empty())
      {
        parent = parentStack.front();
        parentStack.pop_front();
      }

      traverser.pop_front();

      Skeleton::JointMap::iterator curJointIt = Joints.find(cur);
      if(parent != "-1")
      {
        absoluteTransforms[cur] = absoluteTransforms[parent] * curJointIt->second.LocalTransformation ;
        curJointIt->second.Transformation = (absoluteTransforms[cur]) * curJointIt->second.InvBindPose;
      }
      else
      {
        absoluteTransforms[cur] = curJointIt->second.LocalTransformation;
        curJointIt->second.Transformation = absoluteTransforms[cur] * curJointIt->second.InvBindPose;
      }

      for(std::string child : curJointIt->second.Children)
      {
        parentStack.push_back(cur);
        traverser.push_back(child);
      }
    }
  }
}
