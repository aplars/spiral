#ifndef COMPONENT_H
#define COMPONENT_H
#include <memory>

namespace sa {

class Component
{
public:
  virtual ~Component() {}
  Component();
  virtual unsigned int getType() const = 0;
private:
};

typedef std::shared_ptr<Component> ComponentPtr;


template <typename T>
class Component_CRTP : public Component
{
public:
  virtual ~Component_CRTP() {}

  unsigned int getType() const
  {
    return getType_s();
  }

  static unsigned int getType_s()
  {
    static char s_rtti;
    void* kuk = (void*)(&s_rtti);
    return (uintptr_t)(kuk);
  }

private:
};


#define Derive_Component_CRTP(Type) class Type: public Component_CRTP<Type>
}
#endif // COMPONENT_H
