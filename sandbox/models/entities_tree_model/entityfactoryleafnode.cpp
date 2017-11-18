#include "entityfactoryleafnode.h"
#include "entityfactorygroupnode.h"
#include "entityfactorymodel.h"

#include <QDir>
namespace sa {
EntityFactoryLeafNode::EntityFactoryLeafNode(const std::string &dir, const std::string& name)
  : EntityFactoryNode(dir, name)
{ }

EntityFactoryLeafNode::~EntityFactoryLeafNode() {
  delete m_model;
  m_model = nullptr;
}

const EntityFactoryModel *EntityFactoryLeafNode::model() const
{
  return m_model;
}

void EntityFactoryLeafNode::setModel(EntityFactoryModel *model)
{
  m_model = model;

  QDir absPathDir((dir() + "/" + m_model->displayImageName()).c_str());
  QString absPath = absPathDir.absolutePath();
  m_icon.addFile(absPath, QSize(), QIcon::Normal, QIcon::Off);

}

const QIcon& EntityFactoryLeafNode::icon() const
{
  return m_icon;
}

void EntityFactoryLeafNode::print() { std::cout << m_model->displayName() << std::endl; }

}

