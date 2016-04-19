#ifndef VISITORAPPLYANIMATION_H
#define VISITORAPPLYANIMATION_H
#include "visitor.h"
#include "animationmodel.h"
namespace sa {
class VisitorApplyAnimation : public Visitor
{
public:
  VisitorApplyAnimation(float currentTime, const AnimationModel<boost::uuids::uuid> * animation);
  ~VisitorApplyAnimation();

  void visit(TransformationNodeModel* transformationNode);
  void visit(MeshNodeModel* meshNode);
private:
  float m_currentTime;
  const AnimationModel<boost::uuids::uuid > * m_animation;
};
}
#endif // VISITORAPPLYANIMATION_H
