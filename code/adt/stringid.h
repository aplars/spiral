#pragma once
#include <string>
#include <map>

class StringId {
public:
  StringId(const std::string& string) {
    if(m_cache.find(string) == m_cache.end()) {
      m_id = m_counter;
      m_cache[string] = m_id;
      ++m_counter;
    }
    else {
      m_id = m_cache[string];
    }
  }


private:
  unsigned int m_id;
  static unsigned int m_counter = 0;
  static std::map<std::string, unsigned int> m_cache;
};
