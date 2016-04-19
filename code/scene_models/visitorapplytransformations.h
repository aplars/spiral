#pragma once
#include "visitor.h"
#include "transformationnodemodel.h"
#include "meshnodemodel.h"

namespace sa {
class VisitorApplyTransformations : public Visitor
{
public:
  VisitorApplyTransformations();
  ~VisitorApplyTransformations();

  void visit(TransformationNodeModel* transformationNode) {
    sa::Matrix44T<float> old = m_current;
    m_current *= transformationNode->transformation();
    traverse(transformationNode);
    m_current = old;
  }

  void visit(MeshNodeModel* meshNode) {
    meshNode->setTransformation(m_current);
  }

private:
  sa::Matrix44T<float> m_current;
};
}
