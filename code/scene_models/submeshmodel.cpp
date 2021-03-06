#include "submeshmodel.h"
#include <limits>
#include <functional>

namespace sa {
SubMeshModel::SubMeshModel(const std::string& name)
  : m_name(name)
{ }

SubMeshModel::~SubMeshModel() {
}

std::vector<unsigned int> SubMeshModel::getIndices() const {
  std::vector<unsigned int> indices;
  for(const Face& f : m_faces) {
    indices.push_back(f.a);
    indices.push_back(f.b);
    indices.push_back(f.c);
  }
  return indices;
}

void SubMeshModel::addVertex(const SubMeshModel::Vertex &vertex)
{

  glm::vec3 mmin;
  glm::vec3 mmax;

  if(m_vertices.size() <= 0) {
    mmin = glm::vec3(vertex.x, vertex.y, vertex.z);
    mmax = mmin;
  }
  else {
    mmin = m_boundingBox.getMin();
    mmax = m_boundingBox.getMax();
  }
  m_vertices.push_back(vertex);

  if(vertex.x < mmin[0])
    mmin[0] = vertex.x;
  if(vertex.y < mmin[1])
    mmin[1] = vertex.y;
  if(vertex.z < mmin[2])
    mmin[2] = vertex.z;


  if(vertex.x > mmax[0])
    mmax[0] = vertex.x;
  if(vertex.y > mmax[1])
    mmax[1] = vertex.y;
  if(vertex.z > mmax[2])
    mmax[2] = vertex.z;


  m_boundingBox = AABBModel::createFromMinMax(mmin, mmax);

}

void SubMeshModel::addFace(const SubMeshModel::Face &face)
{
  m_faces.push_back(face);
}

void SubMeshModel::setBoneDataForVertex(unsigned int vertexIndex, unsigned int boneId, float weight) {
  for(unsigned int i = 0; i < 4; ++i) {
    if(m_vertices[vertexIndex].w[i] == 0.0f) {
      m_vertices[vertexIndex].b[i] = boneId;
      m_vertices[vertexIndex].w[i] = weight;
      break;
    }
  }
}

const SkeletonPtr SubMeshModel::skeleton() const
{
  return m_skeleton;
}

SkeletonPtr SubMeshModel::skeleton()
{
  return m_skeleton;
}

void SubMeshModel::setSkeleton(SkeletonPtr skeleton)
{
  m_skeleton = skeleton;
}

}
