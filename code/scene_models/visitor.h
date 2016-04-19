#pragma once
namespace sa {
class TransformationNodeModel;
class MeshNodeModel;
class GroupNodeModel;
class Visitor
{
public:
  Visitor();
  virtual ~Visitor();

  virtual void visit(TransformationNodeModel* transformationNode) = 0;
  virtual void visit(MeshNodeModel* meshNode) = 0;

  void traverse(GroupNodeModel* node);
};
}

