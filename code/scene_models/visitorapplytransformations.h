#pragma once
#include "visitor.h"
#include "transformationnodemodel.h"
#include "meshnodemodel.h"
#include <math/mat4ext.h>
namespace sa {
class VisitorApplyTransformations : public Visitor
{
public:
  VisitorApplyTransformations();
  ~VisitorApplyTransformations();

  void visit(TransformationNodeModel* transformationNode) {
    glm::mat4 old = m_current;
    m_current *= transformationNode->transformation();
    traverse(transformationNode);
    m_current = old;
  }

  void visit(MeshNodeModel* meshNode) {
    meshNode->setTransformation(m_current);
  }

private:
  glm::mat4 m_current;
};
}
