#include "backgroundworker.h"
namespace sa {
BackgroundWorker::~BackgroundWorker()
{
  m_threadAlive = false;
  m_condition.notify_one();
  m_thread->join();
}

BackgroundWorker::BackgroundWorker()
  : m_threadAlive(true)
{
  m_thread.reset(new std::thread(&BackgroundWorker::runThread, this));
}


void BackgroundWorker::push(BackgroundWorkPtr work) {
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_work.push_back(work);
  }
  m_condition.notify_one();
}

void BackgroundWorker::runThread() {
  while(m_threadAlive)
  {
    WorkQueue localJobs;
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
