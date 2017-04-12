#include "transformationnodemodel.h"
#include <boost/uuid/uuid_generators.hpp>
#include <QDebug>

namespace sa {
TransformationNodeModel::~TransformationNodeModel()
{}

TransformationNodeModel::TransformationNodeModel()
{}

TransformationNodeModel::TransformationNodeModel(const std::string& name, const glm::mat4& transformation)
  : m_key(boost::uuids::random_generator()())
  , m_name(name)
  , m_transformation(transformation)
{
}
}
