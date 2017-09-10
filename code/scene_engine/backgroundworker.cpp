#include "backgroundworker.h"
namespace sa {
background_worker::~background_worker()
{
  m_condition.notify_one();
  m_thread->join();
}

background_worker::background_worker()
  : m_threadAlive(true)
{
  m_thread.reset(new std::thread(&background_worker::run_thread, this));
}


void background_worker::push(std::shared_ptr<background_worker::work> work) {
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_work.push_back(work);
  }
  m_condition.notify_one();
}

void background_worker::run_thread() {
  while(m_threadAlive)
  {
    work_queue localJobs;
    {
      std::unique_lock<std::mutex> lock(m_mutex);

      //No work need to be done.
      //We just wait so we dont waste any CPU.
      while(m_work.empty() && m_threadAlive)
      {
        m_condition.wait(lock);
      }

      localJobs = m_work;
      m_work.clear();
    }

    while(!localJobs.empty())
    {
      localJobs.back()->doWork_call();
      localJobs.back()->workDone_call();

      localJobs.pop_back();
    }
  }
}
}
