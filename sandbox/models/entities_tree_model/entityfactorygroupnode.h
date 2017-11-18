#pragma once
#include "entityfactorynode.h"
#include <deque>
#include "entityfactoryleafnode.h"
namespace sa {
class EntityFactoryGroupNode : public EntityFactoryNode
{
public:
  EntityFactoryGroupNode(const std::string &dir, const std::string& name);
  virtual ~EntityFactoryGroupNode();
  const std::deque<EntityFactoryNode*>& children() const;
  void addChild(EntityFactoryLeafNode* leaf);
  void addChild(EntityFactoryGroupNode* group);
  const EntityFactoryNode* childAt(unsigned int i) const;
  EntityFactoryNode* childAt(unsigned int i);

  virtual void print()
  {
    EntityFactoryNode::print();
    for(EntityFactoryNode* n : m_children) {
      n->print();
    }
  }

private:
  std::deque<EntityFactoryNode*> m_children;
};
}


