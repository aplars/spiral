#pragma once
#include "visitor.h"
#include <set>
#include "transformationnodemodel.h"

namespace sa {
class VisitorGetMeshNodes : public Visitor
{
public:
  ~VisitorGetMeshNodes();
  VisitorGetMeshNodes();

  void visit(TransformationNodeModel* transformationNode) {
    traverse(transformationNode);
  }

  void visit(MeshNodeModel* meshNode) {
    m_meshes.insert(meshNode);
  }


  std::set<MeshNodeModel*> m_meshes;
};
}
