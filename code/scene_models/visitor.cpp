#include "visitor.h"
#include "groupnodemodel.h"
namespace sa {
Visitor::Visitor()
{

}

Visitor::~Visitor()
{

}

void Visitor::traverse(GroupNodeModel* node) {
  node->traverse(this);
}
}
