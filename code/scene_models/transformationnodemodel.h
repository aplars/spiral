#pragma once
#include "groupnodemodel.h"
#include "visitor.h"
#include <math/Matrix44T.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include<boost/uuid/uuid.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/base_object.hpp>
#include "serialization.h"
#pragma GCC diagnostic pop
#include <deque>
namespace sa {
class TransformationNodeModel : public GroupNodeModel
{
public:
  ~TransformationNodeModel();
  TransformationNodeModel();
  TransformationNodeModel(const std::string& name, const sa::Matrix44T<float>& transformation);

  const boost::uuids::uuid& getKey() const { return m_key; }

  virtual std::string getName() const { return m_name; }

  const sa::Matrix44T<float>& transformation() const {return m_transformation; }
  void setTransformation(const Matrix44T<float>& t) {
    m_transformation = t;
  }

  void accept(Visitor* visitor) { visitor->visit(this); }

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int ) {
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(GroupNodeModel);
    ar & BOOST_SERIALIZATION_NVP(m_key);
    ar & BOOST_SERIALIZATION_NVP(m_name);
    ar & BOOST_SERIALIZATION_NVP(m_transformation);
  }
  boost::uuids::uuid m_key;
  std::string m_name;
  sa::Matrix44T<float> m_transformation;

};
}


