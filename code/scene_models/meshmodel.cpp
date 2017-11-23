#include "meshmodel.h"
#include "meshnodemodel.h"
#include <visitorapplytransformations.h>
#include <visitorgetmeshnodes.h>
#include <visitorapplyanimation.h>

namespace sa {

MeshModel MeshModel::createGroundPlane(int width, int depth) {
  MaterialModel material(
        false,
        MaterialModel::BlendMode::None,
        "",
        "",
        "",
        MaterialModel::TextureMappingMode::Repeat,
        MaterialModel::TextureMappingMode::Repeat,
        MaterialModel::TextureMappingMode::Repeat,
        glm::vec4(1.0f,1.0f,1.0f,1.0f),
        glm::vec4(1.0f,1.0f,1.0f,1.0f),
        glm::vec4(1.0f,1.0f,1.0f,1.0f),
      1,
      10);

  MeshModel model;
  model.setMaterials({material});

  SubMeshModel* subMesh = new SubMeshModel("groundplane");
  subMesh->addVertex(SubMeshModel::Vertex(-width, 0, -depth, 0, 0, 0, 1, 0));
  subMesh->addVertex(SubMeshModel::Vertex( width, 0, -depth, 1, 0, 0, 1, 0));
  subMesh->addVertex(SubMeshModel::Vertex( width, 0,  depth, 1, 1, 0, 1, 0));
  subMesh->addVertex(SubMeshModel::Vertex(-width, 0,  depth, 0, 1, 0, 1, 0));
  subMesh->addFace({2, 1, 0});
  subMesh->addFace({3, 2, 0});
  subMesh->setMaterialKey(0);
  model.setMeshes({subMesh});

  TransformationNodeModel* root = new TransformationNodeModel("root", glm::mat4(1.0f));
  MeshNodeModel* meshNode = new MeshNodeModel(0);
  root->addChild(meshNode);
  model.setTransformationTree(root);

  model.m_header.boundingBox = subMesh->getBoundingBox();
  return model;
}

MeshModel::MeshModel()
{

}

MeshModel::~MeshModel()
{

}

void MeshModel::setMaterials(const MeshModel::Data::Materials& materials) {
  m_data.m_materials = materials;
}


void MeshModel::setMeshes(const std::deque<SubMeshModel*>& meshes) {
  m_data.m_subMeshes = meshes;
}

void MeshModel::setTransformationTree(NodeModel* root) {
  m_data.m_transformationTreeRoot = root;
}

//void MeshModel::setSkeletons(const std::set<Skeleton *> &skeletons)
//{
//  m_data.m_skeletons = skeletons;
//}

void MeshModel::setAnimations(const std::deque<AnimationModel<boost::uuids::uuid> * >& anim) {
  for(AnimationModel<boost::uuids::uuid> * a : anim)
    m_data.m_animations[a->Name] = a;
}

std::deque<std::string> MeshModel::getSkeletalAnimations() const {

//  std::set<std::string> allNamesSet;
//  for(const Skeleton* skeleton : m_data.m_skeletons) {
//    std::deque<std::string> names = skeleton->getAnimationNames();
//    allNamesSet.insert(names.begin(), names.end());
//  }


  std::set<std::string> allNamesSet;
  for(SubMeshModel* sm : m_data.m_subMeshes) {
    if(sm->skeleton()) {
      std::deque<std::string> names = sm->skeleton()->getAnimationNames();
      allNamesSet.insert(names.begin(), names.end());
    }
  }
  std::deque<std::string> allNamesDeque;
  for(std::string n : allNamesSet) {
    allNamesDeque.push_back(n);
  }
  return allNamesDeque;
}

std::deque<std::string> MeshModel::getNodeAnimations() const {
  return m_data.getNodeAnimationNames();
}

const std::set<MeshNodeModel*>& MeshModel::getMeshNodes() {
  if(m_meshNodes.size() <= 0) {
    VisitorGetMeshNodes visitor;
    m_data.m_transformationTreeRoot->accept(&visitor);
    m_meshNodes = visitor.m_meshes;
  }
  return m_meshNodes;
}

bool MeshModel::Data::isLoaded() const
{
  return m_isLoaded;
}
}
