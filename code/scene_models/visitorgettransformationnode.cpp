#include "visitorgettransformationnode.h"
#include "transformationnodemodel.h"
namespace sa {
VisitorGetTransformationNode::~VisitorGetTransformationNode()
{

}


VisitorGetTransformationNode::VisitorGetTransformationNode(const boost::uuids::uuid& nodeToFind)
  : m_nodeToFind(nodeToFind)
  , m_foundNode(nullptr)
{

}

void VisitorGetTransformationNode::visit(TransformationNodeModel* transformationNode) {
  if(m_foundNode == nullptr && transformationNode->getKey() == m_nodeToFind)
    m_foundNode = transformationNode;
  else
    traverse(transformationNode);
}

void VisitorGetTransformationNode::visit(MeshNodeModel* /*meshNode*/) {

}

TransformationNodeModel *VisitorGetTransformationNode::foundNode() const {
  return m_foundNode;
}
}
