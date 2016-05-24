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



/**
 * @brief The DrawData struct contains all information needed to render a mesh.
 */
struct DrawData {
  bool IsVisible = true;
  bool IsTwoSided = true;
  bool IsAlphaBlended = false;
  static const int NUM_TEX_UNITS = 8;
  ShaderUniforms Uniforms;
  IndexBufferPtr IB;
  VertexArrayPtr VAO;
  ShaderProgramPtr SP;
  ShaderProgramPtr SSP;
  TexturePtr TEX[NUM_TEX_UNITS];
  unsigned int NumberOfTrianglesToDraw;
};

//typedef std::deque<DrawData> DrawDataList;
class DrawDataList {
  typedef std::deque<DrawData> DD_List;
  typedef std::set<ShaderProgramPtr> SP_List;
public:
  void push_back(DD_List::const_reference e) {
    m_list.push_back(e);
    m_spList.insert(e.SP);
    m_spList.insert(e.SSP);
  }

  template< class InputIt >
  void insert(DD_List::iterator pos, InputIt first, InputIt last) {
    m_list.insert(pos, first, last);
    std::for_each(first, last, [this](const_reference dd) {
      m_spList.insert(dd.SP);
      m_spList.insert(dd.SSP);

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

  const SP_List& shaders() const { return m_spList; }

  typedef DD_List::const_reference const_reference;
  typedef DD_List::value_type value_type;
private:
  DD_List m_list;
  SP_List m_spList;
};


}
