#include "meshnodemodel.h"
#include <QDebug>

namespace sa {
MeshNodeModel::~MeshNodeModel()
{ }

MeshNodeModel::MeshNodeModel(unsigned int mesh)
  : m_mesh(mesh)
{
}

const sa::Matrix44T<float>& MeshNodeModel::transformation() const
{
  return m_transformation;
}

void MeshNodeModel::setTransformation(const sa::Matrix44T<float> &transformation)
{
  m_transformation = transformation;
}
unsigned int MeshNodeModel::mesh() const
{
    return m_mesh;
}
}

