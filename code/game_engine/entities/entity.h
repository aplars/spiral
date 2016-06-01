#ifndef ENTITY_H
#define ENTITY_H
#include "../components/component.h"
#include <vector>
#include <map>
#include <set>
namespace sa {
class Entity
{
public:
  virtual ~Entity() {}

  Entity();

  template <typename T>
  std::shared_ptr<T> component();
  template <typename T>
  const std::shared_ptr<T> component() const;
  void setComponents(const std::vector<ComponentPtr> &components);
  void addComponent(ComponentPtr component);
  void removeComponent(ComponentPtr component);

private:
  std::map<unsigned int, ComponentPtr> m_components;
};

template <typename T>
std::shared_ptr<T> Entity::component() {
  std::map<unsigned int, ComponentPtr>::const_iterator it = m_components.find(T::getType_s());
  if(it != m_components.end())
    return std::static_pointer_cast<T>(it->second);
  return nullptr;
}

template <typename T>
const std::shared_ptr<T> Entity::component() const {
  return std::static_pointer_cast<T>(m_components.at(T::getType_s()));
}

typedef std::shared_ptr<Entity> EntityPtr;
typedef std::set<EntityPtr> Enteties;
}
#endif // ENTITY_H
