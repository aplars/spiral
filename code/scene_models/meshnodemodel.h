#pragma once
#include "nodemodel.h"
#include "visitor.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/string.hpp>
#include <glm/mat4x4.hpp>
#include "serialization.h"

namespace sa {
class MeshNodeModel : public NodeModel
{
public:
  ~MeshNodeModel();
  MeshNodeModel() {}
  MeshNodeModel(unsigned int mesh);

  virtual std::string getName() const { return "MeshNodeModel"; }
  void accept(Visitor* visitor) { visitor->visit(this); }
  const glm::mat4& transformation() const;
  void setTransformation(const glm::mat4& transformation);

  unsigned int mesh() const;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(NodeModel);
    ar & BOOST_SERIALIZATION_NVP(m_mesh);
  }

  unsigned int m_mesh;
  glm::mat4 m_transformation;
};
}
