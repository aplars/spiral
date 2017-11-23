#pragma once

#include "jointmodel.h"
#include <map>
#include <boost/serialization/map.hpp>
#include <memory>
#include "animationmodel.h"

namespace sa
{
	class Skeleton 
	{
	public:
		Skeleton();
					
		std::string Root;
		
    typedef std::map<std::string, JointModel> JointMap;
		JointMap Joints;

    const JointMap& joints() const { return Joints; }

    std::map<std::string, AnimationModel<std::string> > Animations;

    std::deque<std::string> getAnimationNames() const
    {
      std::deque<std::string> out;
      std::transform(Animations.begin(), Animations.end(), std::back_inserter(out), [](std::map<std::string, AnimationModel<std::string> >::value_type elem)->std::deque<std::string>::value_type {
          return elem.first;
        }
      );
      return out;
    }
    void reset();
    void animate(float currentTime, const std::string& animName);
    void applyTransformations();
	private:
		friend class boost::serialization::access;
		template<class Archive>
    void serialize(Archive &ar, const unsigned int /*version*/)
		{
			ar & BOOST_SERIALIZATION_NVP(Root);
			ar & BOOST_SERIALIZATION_NVP(Joints);
      ar & BOOST_SERIALIZATION_NVP(Animations);
    }
  };
  typedef std::shared_ptr<Skeleton> SkeletonPtr;
}
