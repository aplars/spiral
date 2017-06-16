#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include<boost/uuid/uuid.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/serialization/string.hpp>
#include "serialization.h"
#pragma GCC diagnostic pop

#include <deque>
namespace sa {
class Visitor;
class NodeModel
{
public:
  NodeModel() { }
  virtual ~NodeModel() {}
  virtual std::string getName() const = 0;
  virtual void accept(Visitor* visitor) = 0;
protected:
  std::string m_dummy;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int ) {
    ar & BOOST_SERIALIZATION_NVP(m_dummy);
  }

};
}
