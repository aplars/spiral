#pragma once
#include "indexbuffer.h"
#include "vertexarray.h"
#include "shaderprogram.h"
#include "texture.h"
#include <map>
#include <deque>
#include <algorithm>
#include "shaderuniforms.h"

namespace sa {

enum class Blending {
  None,
  Normal,
  Addative,
  SRC_ALPHA_ONE
};

enum class PolygonMode {
  Wire,
  Fill
};

enum class DrawPrimitive {
  Triangles,
  TriangleStrip,
};

enum class Alpha {
  None,
  Greater,
  Less
};

/**
 * @brief The DrawData struct contains all information needed to render a mesh.
 */
struct DrawData {
  bool IsVisible = true;
  bool IsTwoSided = true;
  Blending BlendingFunction = Blending::Normal;
  Alpha AlphaFunction = Alpha::None;
  float AlphaValue = 0.0f;

  PolygonMode PolygonDrawMode = PolygonMode::Fill;
  DrawPrimitive Primitive = DrawPrimitive::Triangles;
  static const int NUM_TEX_UNITS = 16;
  ShaderUniforms Uniforms;
  IndexBufferPtr IB;
  VertexArrayPtr VAO;
  ShaderProgramPtr Current_SP;
  ShaderProgramPtr Uber_SP;
  ShaderProgramPtr ShadowMap_SP;
  ShaderProgramPtr SunLightShafts_SP;

  TexturePtr TEX[NUM_TEX_UNITS];
  unsigned int NumberOfTrianglesToDraw;
};

class DrawDataList {
  typedef std::deque<DrawData> DD_List;
  typedef std::set<ShaderProgramPtr> SP_List;
public:
  void push_back(DD_List::const_reference e) {
    m_list.push_back(e);
    m_spList.insert(e.Current_SP);
    m_spList.insert(e.Uber_SP);
    m_spList.insert(e.ShadowMap_SP);
    m_spList.insert(e.SunLightShafts_SP);
  }

  template< class InputIt >
  void insert(DD_List::iterator pos, InputIt first, InputIt last) {
    m_list.insert(pos, first, last);
    std::for_each(first, last, [this](const_reference dd) {
      m_spList.insert(dd.Current_SP);
      m_spList.insert(dd.Uber_SP);
      m_spList.insert(dd.ShadowMap_SP);
      m_spList.insert(dd.SunLightShafts_SP);
    });
  }

  template< class InputIt >
  void insert(DD_List::const_iterator pos, InputIt first, InputIt last) {
    m_list.insert(pos, first, last);
  }

  void clear() {
    m_list.clear();
    m_spList.clear();
  }

  size_t size() const { return m_list.size(); }

  DD_List::const_iterator begin() const {
    return m_list.begin();
  }

  DD_List::iterator begin() {
    return m_list.begin();
  }

  DD_List::const_iterator end() const {
    return m_list.end();
  }

  DD_List::iterator end() {
    return m_list.end();
  }

  DD_List::reference operator[](int i) { return m_list[i]; }
  DD_List::const_reference operator[](int i) const { return m_list[i]; }

  const SP_List& shaders() const { return m_spList; }

  typedef DD_List::const_reference const_reference;
  typedef DD_List::value_type value_type;
private:
  DD_List m_list;
  SP_List m_spList;
};


}
