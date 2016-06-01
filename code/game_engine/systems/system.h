#ifndef SYSTEM_H
#define SYSTEM_H
#include "../entities/entity.h"
namespace sa {
class System
{
public:
  System();
  virtual void update(float dt, const Enteties& gameObjects) = 0;
  //Subject<GameObjectPtr> RemoveEntitySubject;
  //Subject<ProducerT> AddEntitySubject;
};
}
#endif // SYSTEM_H
