#include "entity.h"
#include <algorithm>
namespace sa {
Entity::Entity()
{

}

void Entity::setComponents(const std::vector<ComponentPtr> &components){
  std::for_each(components.begin(), components.end(), [=](ComponentPtr component) {
    m_components[component->getType()] = component;
  });
}

void Entity::addComponent(ComponentPtr component) {
  unsigned int thetype = component->getType();
  m_components[thetype] = component;
}

void Entity::removeComponent(ComponentPtr component) {
  unsigned int thetype = component->getType();
  m_components.erase(thetype);
}
}
