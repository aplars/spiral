#include "entityfactorynode.h"
namespace sa {
EntityFactoryNode::EntityFactoryNode(const std::string &dir, const std::string &name)
  : m_dir(dir)
  , m_name(name)
{ }

void EntityFactoryNode::setParent(EntityFactoryNode *parent)
{
  m_parent = parent;
}

EntityFactoryNode *EntityFactoryNode::parent() const
{
    return m_parent;
}

std::string EntityFactoryNode::dir() const
{
  return m_dir;
}

std::string EntityFactoryNode::name() const
{
  return m_name;
}
}
