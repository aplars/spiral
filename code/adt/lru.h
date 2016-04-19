#pragma once
#include <map>
#include <list>

namespace sa
{
///An lru cache storing key value pairs.
template <typename K, typename V>
class lru
{
public:

  typedef std::pair<K, V> key_value_type;
  typedef std::list<std::pair<K, V>> history_type;
  typedef std::map<K, typename history_type::iterator> key_value_map_type;

  typedef typename history_type::iterator iterator;
  typedef typename history_type::const_iterator const_iterator;
  typedef typename history_type::value_type value_type;


  lru(std::size_t capacity)
    : m_capacity(capacity) {}

  iterator begin() { return m_history.begin(); }
  const_iterator begin() const { return m_history.begin(); }

  iterator end() { return m_history.end(); }
  const_iterator end() const { return m_history.end(); }

  typename history_type::size_type size() const
  {
    return m_history.size();
  }


  void insert(const K& key, const V& value)
  {
    typename key_value_map_type::iterator it = m_key_value_map.find(key);
    if(it != m_key_value_map.end())
    {
      //already exist. Make it used.
      K k = it->first;
      V v = it->second->second;
      m_history.erase(it->second);
      m_history.push_front(key_value_type(k, v));
      m_key_value_map[k] = m_history.begin();
    }
    else
    {
      m_history.push_front(key_value_type(key, value));
      m_key_value_map[key] = m_history.begin();
    }
    //remove objects until we reach the budget.
    if(m_history.size() > m_capacity)
    {
      while(m_history.size() > m_capacity)
      {
        typename history_type::iterator removeIt = m_history.end();
        --removeIt;

        m_key_value_map.erase(removeIt->first);
        removeIt = m_history.erase(removeIt);
      }
    }
  }

  bool exist(const K& key) const
  {
    return m_key_value_map.find(key) != m_key_value_map.end();
  }

  bool try_get(const K& key, V& value)
  {
    typename key_value_map_type::iterator it = m_key_value_map.find(key);
    if(it != m_key_value_map.end())
    {
      K k = it->first;
      V v = it->second->second;

      m_history.erase(it->second);
      m_history.push_front(key_value_type(k, v));
      value = m_history.begin()->second;
      m_key_value_map[key] = m_history.begin();
      return true;
    }
    return false;
  }

  bool try_get_no_prio_change(const K& key, V& value)
  {
    typename key_value_map_type::iterator it = m_key_value_map.find(key);
    if(it != m_key_value_map.end())
    {
      V v = it->second->second;
      value = v;

      return true;
    }
    return false;
  }

  void touch(const K& key)
  {
    V val;
    try_get(key, val);
  }
private:
  std::size_t m_capacity;
  key_value_map_type m_key_value_map;
  history_type m_history;
};
}
