#pragma once
#include "visitor.h"
#include <set>
#include "transformationnodemodel.h"

namespace sa {
class VisitorGetMeshNodesUnderSpecificTransformation : public Visitor
{
public:
  VisitorGetMeshNodesUnderSpecificTransformation(const std::string& nodeName)
    : m_nodeName(nodeName)
  {

  }

  ~VisitorGetMeshNodesUnderSpecificTransformation();

  void visit(TransformationNodeModel* transformationNode) {
    if(m_nodeName == transformationNode->getName()) {
      collect = true;
    }
    traverse(transformationNode);
    collect = false;
  }

  void visit(MeshNodeModel* meshNode) {
    if(collect == true)
      m_meshes.insert(meshNode);
  }

  bool collect = false;
  std::string m_nodeName;
  std::set<MeshNodeModel*> m_meshes;

};
}
