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


  ~background_worker();
  background_worker();

  void push(std::shared_ptr<background_worker::work> work);

private:
  void run_thread();

  typedef std::unique_ptr<std::thread> thread_ptr;
  thread_ptr m_thread;

  typedef std::deque<std::shared_ptr<background_worker::work>> work_queue;

  bool m_threadAlive = false;

  std::mutex m_mutex;
  std::condition_variable m_condition;

  work_queue m_work;
};


}
