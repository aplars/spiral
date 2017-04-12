#pragma once
#include <set>
#include <vector>
#include <string>
#include <glm/mat4x4.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>

namespace sa
{
  class JointModel
	{
	public:
		class VertexWeight
		{
		public:
			VertexWeight() {}
			VertexWeight(unsigned int i, float w)
				: VertexIndex(i)
				, Weight(w) {}

			unsigned int VertexIndex;
			float Weight;

		private:
			friend class boost::serialization::access;
			template<class Archive>
      void serialize(Archive &ar, const unsigned int /*version*/)
			{
				ar & BOOST_SERIALIZATION_NVP(VertexIndex);
				ar & BOOST_SERIALIZATION_NVP(Weight);
			}
		};
    JointModel() {}
    JointModel(const std::string& name, const glm::mat4& invBindPose, const glm::mat4& localTransformation, const std::vector<VertexWeight>& weights, std::string parentIndex = "-1");
				
    std::string Name;

    glm::mat4 InvBindPose;

    glm::mat4 LocalTransformation;

    glm::mat4 Transformation;

    typedef std::vector<VertexWeight> VertexWeights;
    VertexWeights Weights;

    std::string Parent;
		std::set<std::string> Children;

		


	private:
		friend class boost::serialization::access;
		template<class Archive>
    void serialize(Archive &ar, const unsigned int )
		{
			ar & BOOST_SERIALIZATION_NVP(Parent);
			ar & BOOST_SERIALIZATION_NVP(Children);
			ar & BOOST_SERIALIZATION_NVP(InvBindPose);
			ar & BOOST_SERIALIZATION_NVP(LocalTransformation);
			ar & BOOST_SERIALIZATION_NVP(Name);
			ar & BOOST_SERIALIZATION_NVP(Weights);
		}
	};
}
