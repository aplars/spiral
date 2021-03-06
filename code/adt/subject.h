#pragma once
#include <map>
#include <functional>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>

namespace sa {
typedef int observer_id;

template <class... Args>
class subject {
public:
  typedef std::function<void(Args...)> function;
  typedef std::map<observer_id, function> function_map;
  observer_id m_currentId = 0;

  subject() {}

  observer_id registerObserver(const function& function) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_functions.insert(std::make_pair(m_currentId, function));
    ++m_currentId;
    return m_currentId-1;
  }

  bool unregisterObserver(observer_id id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_functions.erase(id) > 0;
  }

  void notify(Args... a) {
    for(typename function_map::value_type vt : m_functions) {
      function f = vt.second;
      f(a...);
    }
  }

  observer_id operator+=(const function& function) {
    return registerObserver(function);
  }

private:
  subject(const subject&);
  subject operator=(const subject&);
  mutable std::mutex m_mutex;
  function_map m_functions;
};
}
