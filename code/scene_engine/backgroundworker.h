#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>
#include "streamedmeshentity.h"

namespace sa {
class BackgroundWork {

public:
  BackgroundWork(StreamedMeshEntity* entity) {
    m_entity = entity;
  }

  std::function<void(StreamedMeshEntity*)> doWork = nullptr;
  std::function<void(StreamedMeshEntity*)> workDone = nullptr;

  void doWork_call() {
    if(doWork)
      doWork(m_entity);
  }

  void workDone_call() {
    if(workDone)
      workDone(m_entity);
  }


  StreamedMeshEntity* m_entity;
};

typedef std::shared_ptr<BackgroundWork> BackgroundWorkPtr;

class BackgroundWorker
{
public:
  ~BackgroundWorker();
  BackgroundWorker();

  void push(BackgroundWorkPtr work);

private:
  void runThread();

  typedef std::unique_ptr<std::thread> ThreadScopedPtr;
  ThreadScopedPtr m_thread;

  typedef std::deque<BackgroundWorkPtr> WorkQueue;

  bool m_threadAlive;

  std::mutex m_mutex;
  std::condition_variable m_condition;

  WorkQueue m_work;
};
}
