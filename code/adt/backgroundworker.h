#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <functional>

namespace sa {

/**
 * @brief The background_worker class runs tasks (do work) in the background.
 * The user makes an instance of background_worker and adds work to it with
 * the push method. The actual work is executed in the doWork function. When
 * the work is completed the workDone function is called. Its u to the user to
 * specify doWork and workDone.
 */
class background_worker
{
public:
  /**
   * @brief The work class represents the actual work. Its just two user defined
   * functions. One that is called to do the work and one that is called when the
   * work is completed.
   *
   * Example usage:
   * \code{.cpp}
   * background_worker::work work;
   * \endcode
   */
  class work {
  public:
    work() {
    }

    std::function<void()> doWork = nullptr;
    std::function<void()> workDone = nullptr;

    void doWork_call() {
      if(doWork)
        doWork();
    }

    void workDone_call() {
      if(workDone)
        workDone();
    }

  };
  typedef std::shared_ptr<work> work_ptr;


  ~background_worker()
  {
    m_threadAlive = false;
    m_condition.notify_one();
    m_thread->join();
  }

  background_worker()
    : m_threadAlive(true)
  {
    m_thread.reset(new std::thread(&background_worker::run_thread, this));
  }

  void push(const background_worker::work& work) {
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_work.push_back(work);
    }
    m_condition.notify_one();
  }

private:
  background_worker(const background_worker&);
  background_worker operator=(const background_worker&);
  void run_thread() {
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
        localJobs.back().doWork_call();
        localJobs.back().workDone_call();

        localJobs.pop_back();
      }
    }
  }

  typedef std::unique_ptr<std::thread> thread_ptr;
  thread_ptr m_thread;

  typedef std::deque<background_worker::work> work_queue;

  bool m_threadAlive = false;

  std::mutex m_mutex;
  std::condition_variable m_condition;

  work_queue m_work;
};
}
