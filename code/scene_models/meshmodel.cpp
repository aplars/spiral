#include "meshmodel.h"
#include "meshnodemodel.h"
#include <visitorapplytransformations.h>
#include <visitorgetmeshnodes.h>
#include <visitorapplyanimation.h>

namespace sa {

MeshModel MeshModel::createGroundPlane(int width, int depth) {
  MaterialModel* material = new MaterialModel(
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

  TransformationNodeModel* root = new TransformationNodeModel("root", Matrix44T<float>::GetIdentity());
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

void MeshModel::setMaterials(const std::deque<MaterialModel*>& materials) {
  m_data.m_materials = materials;
}


void MeshModel::setMeshes(const std::deque<SubMeshModel*>& meshes) {
  m_data.m_subMeshes = meshes;
}

//void MeshModel::calculateBoundingBoxForAllAnimations() {
//  float currentTime = 0.0f;
//  if(m_data.m_animations.size() > 0) {
//    while(m_data.m_animations[m_data.getAnimationNames()[0]]->Duration < currentTime) {
//      VisitorApplyAnimation* visitor = new VisitorApplyAnimation(currentTime, m_data.m_animations[m_data.getAnimationNames()[0]]);
//      m_data.m_transformationTreeRoot->accept(visitor);
//    }
//    calculateBoundingBoxAfterTransformations();
//    currentTime+=1/60.0f;
//  }


//  if(m_data.m_haveBones && m_data.m_animations.size() > 0) {
//    currentTime = 0.0f;
//    while(m_data.m_animations[m_data.getAnimationNames()[0]]->Duration < currentTime) {
//      for(sa::MeshModel::Data::SubMeshes::value_type sm : m_data.m_subMeshes) {
//        SubMeshModel* subMesh = sm.second;
//        if(subMesh->skeleton())
//          subMesh->skeleton()->animate(currentTime, subMesh->skeleton()->getAnimationNames()[0]);
//      }
//      calculateBoundingBoxAfterTransformations();
//      currentTime+=1/60.0f;
//    }
//  }
//}


//void MeshModel::calculateBoundingBoxAfterTransformations() {
//  if(m_data.m_transformationTreeRoot) {
//    VisitorApplyTransformations transformvisitor;
//    m_data.m_transformationTreeRoot->accept(&transformvisitor);

//    VisitorGetMeshNodes getvisitor;
//    m_data.m_transformationTreeRoot->accept(&getvisitor);
//    for(MeshNodeModel* mesh : getvisitor.m_meshes) {
//      AABBModel subBoundingBox =  m_data.m_subMeshes[mesh->mesh()]->getBoundingBox();
//      subBoundingBox.transform(mesh->transformation());
//      m_header.boundingBox.expand(subBoundingBox);
//    }
//  }
//}

//void MeshModel::calculateBoundingBox() {
//  MeshModel::Data::SubMeshes::iterator firstSubMeshIt = m_data.m_subMeshes.begin();
//  m_header.boundingBox =  firstSubMeshIt->second->getBoundingBox();
//  //calculateBoundingBoxForAllAnimations();
//  calculateBoundingBoxAfterTransformations();
//}

void MeshModel::setTransformationTree(NodeModel* root) {
  m_data.m_transformationTreeRoot = root;
}

void MeshModel::setAnimations(const std::deque<AnimationModel<boost::uuids::uuid> * >& anim) {
  for(AnimationModel<boost::uuids::uuid> * a : anim)
    m_data.m_animations[a->Name] = a;
}

std::deque<std::string> MeshModel::getSkeletalAnimations() const {
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
}
