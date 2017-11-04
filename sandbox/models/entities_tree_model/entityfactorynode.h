#pragma once

#include <string>
#include <iostream>
namespace sa {
class EntityFactoryNode
{
public:
  virtual ~EntityFactoryNode() {}
  EntityFactoryNode(const std::string &dir, const std::string& name);
  void setParent(EntityFactoryNode*parent);
  EntityFactoryNode *parent() const;

  std::string dir() const;

  std::string name() const;

  virtual void print() { std::cout << m_name << std::endl; }
protected:
private:
  std::string m_dir;
  std::string m_name;
  EntityFactoryNode* m_parent = nullptr;
};
}
