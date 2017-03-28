#pragma once
#include <math/Matrix44T.h>
#include <math/QuaternionT.h>
#include <boost/serialization/nvp.hpp>

namespace boost {
namespace serialization {

template<class Archive, typename T>
void serialize(Archive & ar, sa::Matrix44T<T> & matrix, const unsigned int )
{
  ar & BOOST_SERIALIZATION_NVP(matrix.m_E);
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

