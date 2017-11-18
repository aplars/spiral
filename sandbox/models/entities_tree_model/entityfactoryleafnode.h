#pragma once

#include "entityfactorynode.h"
#include <string>
#include <QIcon>

namespace sa {
class EntityFactoryGroupNode;
class EntityFactoryModel;

class EntityFactoryLeafNode : public EntityFactoryNode
{
public:
  EntityFactoryLeafNode(const std::string& dir, const std::string& name);
  virtual ~EntityFactoryLeafNode();
  const EntityFactoryModel *model() const;
  void setModel(EntityFactoryModel *model);

  const QIcon &icon() const;

  virtual void print();

private:
  EntityFactoryModel* m_model;
  QIcon m_icon;
};

}
