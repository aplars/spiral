#include "jointmodel.h"

namespace sa
{
  JointModel::JointModel(const std::string& name, const glm::mat4& invBindPose, const glm::mat4& localTransformation, const std::vector<VertexWeight>& weights, std::string parentIndex)
		: Name(name)
		, InvBindPose(invBindPose)
		, LocalTransformation(localTransformation)
		, Weights(weights)
		, Parent(parentIndex)
	{
		
	}


}
