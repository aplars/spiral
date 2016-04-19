#include "groupnodemodel.h"
#include "visitor.h"
#include <QDebug>
namespace sa {

void GroupNodeModel::addChild(NodeModel* child) {
  m_children.push_back(child);
}

void GroupNodeModel::traverse(Visitor* visitor) {
  for(NodeModel* node : m_children) {
    node->accept(visitor);
  }
}
}

