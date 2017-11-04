#pragma once
#include "entityfactorygroupnode.h"
namespace sa {
class EntityFactoryRootNode : public EntityFactoryGroupNode
{
public:
  EntityFactoryRootNode(const std::string &dir, const std::string& name);


  static EntityFactoryGroupNode *loadFromDisk(const std::string &entitiesDir);

private:
  static EntityFactoryModel *loadJson(const std::string &dir, const std::string& file);

};
}
