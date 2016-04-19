#pragma once
#include "materialmodel.h"
#include "submeshmodel.h"
#include "transformationnodemodel.h"
#include "animationmodel.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/map.hpp>
#pragma GCC diagnostic pop
#include <deque>
#include <map>

namespace sa {
//class MeshNodeModel;
/**
 * @brief The MeshModel class is a collection of objects needed to form a renderable object.
 * The mesh is grouped into several sub-meshes, each with its own material. The mesh store
 * A transformation-hierachy. This hierarchy is used to place individual sub-meshes relative to eachother.
 * Animatons can be used to update the nodes in the herarchy periodically.
 *
 * To be able to support 'on demand loading' we split the mesh into a header- and a data-block.
 * We can use this property by first loading the header. Check the bounding-box and then load the data if needed.
 */
class MeshModel
{
public:
  /**
   * @brief The Header struct is intended to be used to determine if the Data needs to be loaded.
   */
  struct Header {
    AABBModel boundingBox;
    std::deque<std::string> skeletalAnimations;
    std::deque<std::string> nodeAnimations;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int ) {
      ar & BOOST_SERIALIZATION_NVP(boundingBox);
      ar & BOOST_SERIALIZATION_NVP(skeletalAnimations);
      ar & BOOST_SERIALIZATION_NVP(nodeAnimations);

    }
  };

  /**
   * @brief The Data struct is the actual mesh. I contain sub-meshes, a transformation-tree to update
   * the orientations and the positions of the sub-meshes, the materials referenced by the sub-meshes and
   * animations that can update the nodes in the transformation-tree.
   */
  struct Data {
    typedef std::deque<SubMeshModel* > SubMeshes;
    typedef std::deque<MaterialModel*> Materials;
    typedef std::map<std::string, AnimationModel<boost::uuids::uuid >* > Animations;
    bool m_haveBones = false;
    /**
     * @brief m_materials is a collection of all the mesh's materials.
     */
    Materials m_materials;
    /**
     * @brief m_subMeshes is a collection of triangle meshes all referencing their own material.
     */
    SubMeshes m_subMeshes;
    /**
     * @brief m_transformationTreeRoot is used to place sub-meshes relative eachother.
     */
    NodeModel* m_transformationTreeRoot = nullptr;
    /**
     * @brief m_animations is a collection of animations that can be used to update the transformation-tree.
     */
    Animations m_animations;
    std::deque<std::string> getNodeAnimationNames() const {
      std::deque<std::string> names;
      for(Animations::value_type animation : m_animations) {
        names.push_back(animation.first);
      }
      return names;
    }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int ) {
      ar & BOOST_SERIALIZATION_NVP(m_haveBones);
      ar & BOOST_SERIALIZATION_NVP(m_materials);
      ar & BOOST_SERIALIZATION_NVP(m_subMeshes);
      ar & BOOST_SERIALIZATION_NVP(m_transformationTreeRoot);
      ar & BOOST_SERIALIZATION_NVP(m_animations);
    }

    void unload() {
      m_materials.clear();
      m_subMeshes.clear();
      delete m_transformationTreeRoot;
      m_transformationTreeRoot = nullptr;
      m_animations.clear();
    }
  };
  static MeshModel createGroundPlane(int width, int depth);

  MeshModel();
  ~MeshModel();

  void setHaveBones(bool haveBones) { m_data.m_haveBones = haveBones; }
  void setMaterials(const std::deque<MaterialModel*>& materials);
  void setMeshes(const std::deque<SubMeshModel*>& meshes);
  void setTransformationTree(NodeModel* root);
  void setAnimations(const std::deque<AnimationModel<boost::uuids::uuid> * >& anim);
  std::deque<std::string> getSkeletalAnimations() const;
  std::deque<std::string> getNodeAnimations() const;
  const std::set<MeshNodeModel*>& getMeshNodes();
  void unload() { m_data.unload(); }
  Header m_header;
  Data m_data;
private:
  void calculateBoundingBox() ;
  void calculateBoundingBoxAfterTransformations() ;
  void calculateBoundingBoxForAllAnimations() ;

  std::set<MeshNodeModel*> m_meshNodes;

};
}

