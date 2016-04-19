#include "jointmodel.h"

namespace sa
{
  JointModel::JointModel(const std::string& name, const Matrix44T<float>& invBindPose, const Matrix44T<float>& localTransformation, const std::vector<VertexWeight>& weights, std::string parentIndex)
		: Name(name)
		, InvBindPose(invBindPose)
		, LocalTransformation(localTransformation)
		, Weights(weights)
		, Parent(parentIndex)
	{
		
	}


}
