#pragma once

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include "serialization.h"
#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include "aabbmodel.h"
#include "skeletonmodel.h"

namespace sa {
class SubMeshModel
{
public:

  struct Vertex {
    Vertex() {}
    Vertex(float xx, float yy, float zz, float uu, float vv, float nnx, float nny, float nnz)
      : x(xx), y(yy), z(zz), u(uu), v(vv), nx(nnx), ny(nny), nz(nnz) { normalizeNormal(); }

    float operator[](int i) const {
      if(i == 0) return x;
      if(i == 1) return y;
      return z;
    }

    void setPosition(const glm::vec3& p) {
      x = p[0];
      y = p[1];
      z = p[2];
    }

    void setNormal(const glm::vec3& p) {
      nx = p[0];
      ny = p[1];
      nz = p[2];
    }

    void setWeight(unsigned int bb, float ww) {
      for(int i = 0; i < 4; ++i) {
        if(w[i] == 0.0f) {
          w[i] = ww;
          b[i] = bb;
          break;
        }
      }
    }

    glm::vec3 position() const {
      return glm::vec3(x, y, z);
    }

    glm::vec3 normal() const {
      return glm::vec3(nx, ny, nz);
    }


    void incPosition(const glm::vec3& p) {
      x += p[0];
      y += p[1];
      z += p[2];
    }

    void incNormal(const glm::vec3& p) {
      nx += p[0];
      ny += p[1];
      nz += p[2];
    }

    void normalizeNormal() {
      glm::vec3 n(nx, ny, nz);
      setNormal(glm::normalize(n));
    }

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int ) {
      ar & BOOST_SERIALIZATION_NVP(x);
      ar & BOOST_SERIALIZATION_NVP(y);
      ar & BOOST_SERIALIZATION_NVP(z);

      ar & BOOST_SERIALIZATION_NVP(u);
      ar & BOOST_SERIALIZATION_NVP(v);

      ar & BOOST_SERIALIZATION_NVP(nx);
      ar & BOOST_SERIALIZATION_NVP(ny);
      ar & BOOST_SERIALIZATION_NVP(nz);

      ar & BOOST_SERIALIZATION_NVP(b);

      ar & BOOST_SERIALIZATION_NVP(w);
    }
    float x, y, z;
    float u, v;
    float nx, ny, nz;
    float b[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    float w[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  };

  struct Face {
    Face() {}
    Face(unsigned int aa, unsigned int bb, unsigned int cc)
      : a(aa), b(bb), c(cc) {}

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int ) {
      ar & BOOST_SERIALIZATION_NVP(a);
      ar & BOOST_SERIALIZATION_NVP(b);
      ar & BOOST_SERIALIZATION_NVP(c);
    }
    unsigned int a, b, c;
  };

  SubMeshModel() {}
  SubMeshModel(const std::string& name);

  const AABBModel& getBoundingBox() const { return m_boundingBox; }
  void setBoundingBox(const AABBModel& bbox) { m_boundingBox = bbox; }
  const std::vector<Vertex>& getVertices() const { return m_vertices; }
  const std::vector<Face>& getFaces() const { return m_faces; }
  std::vector<unsigned int> getIndices() const;
  unsigned int getMaterialKey() const { return m_materialKey; }
  void addVertex(const Vertex& vertex);
  void addFace(const Face& face);

  void setMaterialKey(unsigned int key) { m_materialKey = key; }

  void setBoneDataForVertex(unsigned int vertexIndex,
                            unsigned int boneId,
                            float weight) {
    for(unsigned int i = 0; i < 4; ++i) {
      if(m_vertices[vertexIndex].w[i] == 0.0f) {
        m_vertices[vertexIndex].b[i] = boneId;
        m_vertices[vertexIndex].w[i] = weight;
        break;
      }
    }
  }

  const Skeleton* skeleton() const;
  Skeleton* skeleton();
  void setSkeleton(Skeleton* skeleton);

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    (void)version;
    ar & BOOST_SERIALIZATION_NVP(m_name);
    ar & BOOST_SERIALIZATION_NVP(m_faces);
    ar & BOOST_SERIALIZATION_NVP(m_vertices);
    ar & BOOST_SERIALIZATION_NVP(m_materialKey);
    ar & BOOST_SERIALIZATION_NVP(m_skeleton);
    ar & BOOST_SERIALIZATION_NVP(m_boundingBox);
  }

  std::string m_name;
  std::vector<Face> m_faces;
  std::vector<Vertex> m_vertices;
  unsigned int m_materialKey;
  Skeleton* m_skeleton = nullptr;
  AABBModel m_boundingBox;
};
}

