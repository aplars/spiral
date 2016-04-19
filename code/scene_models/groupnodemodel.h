#ifndef GROUPNODEMODEL_H
#define GROUPNODEMODEL_H
#include "nodemodel.h"
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

namespace sa {
class GroupNodeModel : public NodeModel
{
public:
  virtual ~GroupNodeModel()  {
    for(NodeModel* child : m_children) {
      delete child;
      child = nullptr;
    }
    m_children.clear();
  }

  GroupNodeModel() {}
  void addChild(NodeModel* child);
  virtual std::string getName() const { return "GroupNodeModel"; }
  void traverse(Visitor* visitor);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NodeModel);
    ar & BOOST_SERIALIZATION_NVP(m_children);
  }

  std::deque<NodeModel*> m_children;
};
}
#endif // GROUPNODEMODEL_H
