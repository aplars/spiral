#pragma once
#include <boost/uuid/uuid.hpp>
#include "visitor.h"

namespace sa {
class VisitorGetTransformationNode : public Visitor
{
public:
  ~VisitorGetTransformationNode();
  VisitorGetTransformationNode(const boost::uuids::uuid& nodeToFind);
  void visit(TransformationNodeModel* transformationNode);
  void visit(MeshNodeModel* meshNode);

  TransformationNodeModel *foundNode() const;

private:
  boost::uuids::uuid m_nodeToFind;
  TransformationNodeModel* m_foundNode = nullptr;
};
}

