#pragma once
#include <math/QuaternionT.h>
#include <boost/serialization/nvp.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, glm::mat4 & matrix, const unsigned int )
{
  ar & BOOST_SERIALIZATION_NVP(matrix[0]);
  ar & BOOST_SERIALIZATION_NVP(matrix[1]);
  ar & BOOST_SERIALIZATION_NVP(matrix[2]);
  ar & BOOST_SERIALIZATION_NVP(matrix[3]);
}

template<class Archive>
void serialize(Archive & ar, glm::vec3 & vector, const unsigned int )
{
  ar & BOOST_SERIALIZATION_NVP(vector[0]);
  ar & BOOST_SERIALIZATION_NVP(vector[1]);
  ar & BOOST_SERIALIZATION_NVP(vector[2]);
}

template<class Archive>
void serialize(Archive & ar, glm::vec4 & vector, const unsigned int )
{
  ar & BOOST_SERIALIZATION_NVP(vector[0]);
  ar & BOOST_SERIALIZATION_NVP(vector[1]);
  ar & BOOST_SERIALIZATION_NVP(vector[2]);
  ar & BOOST_SERIALIZATION_NVP(vector[3]);
}

template<class Archive, typename T>
void serialize(Archive & ar, sa::QuaternionT<T> & q, const unsigned int )
{
  ar & BOOST_SERIALIZATION_NVP(q.m_v);
  ar & BOOST_SERIALIZATION_NVP(q.m_s);
}
} // namespace serialization
} // namespace boost

