#include "entityfactorygroupnode.h"
namespace sa {
EntityFactoryGroupNode::EntityFactoryGroupNode(const std::string &dir, const std::string &name)
  : EntityFactoryNode(dir, name)
{

}

EntityFactoryGroupNode::~EntityFactoryGroupNode() {
  for(EntityFactoryNode* c : m_children) {
    delete c;
    c = nullptr;
  }
  m_children.clear();;
}

const std::deque<EntityFactoryNode *>& EntityFactoryGroupNode::children() const
{
  return m_children;
}

void EntityFactoryGroupNode::addChild(EntityFactoryLeafNode *leaf)
{
  m_children.push_back(leaf);
  leaf->setParent(this);
}

void EntityFactoryGroupNode::addChild(EntityFactoryGroupNode *group)
{
  m_children.push_back(group);
  group->setParent(this);
}

const EntityFactoryNode *EntityFactoryGroupNode::childAt(unsigned int i) const
{
  return m_children.at(i);
}

EntityFactoryNode *EntityFactoryGroupNode::childAt(unsigned int i)
{
  return m_children.at(i);
}
}
