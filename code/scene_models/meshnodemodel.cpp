#include "meshnodemodel.h"
#include <QDebug>

namespace sa {
MeshNodeModel::~MeshNodeModel()
{ }

MeshNodeModel::MeshNodeModel(unsigned int mesh)
  : m_mesh(mesh)
{
}

const glm::mat4& MeshNodeModel::transformation() const
{
  return m_transformation;
}

void MeshNodeModel::setTransformation(const glm::mat4& transformation)
{
  m_transformation = transformation;
}
unsigned int MeshNodeModel::mesh() const
{
    return m_mesh;
}
}

