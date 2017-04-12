#include "visitorapplyanimation.h"
#include "transformationnodemodel.h"
namespace sa {
VisitorApplyAnimation::VisitorApplyAnimation(float currentTime, const AnimationModel<boost::uuids::uuid > * animation)
  : m_currentTime(currentTime)
  , m_animation(animation)
{

}

VisitorApplyAnimation::~VisitorApplyAnimation()
{

}

void VisitorApplyAnimation::visit(TransformationNodeModel* transformationNode) {
  float durationTime = m_animation->Duration;
  float currentTimeRepeated = fmod(m_currentTime, durationTime);

  glm::mat4 transformation;
  if(m_animation->getTransformation(currentTimeRepeated, transformationNode->getKey(), transformation)) {
    transformationNode->setTransformation(transformation);
  }
  traverse(transformationNode);
}

void VisitorApplyAnimation::visit(MeshNodeModel* /*meshNode*/) {
}
}
