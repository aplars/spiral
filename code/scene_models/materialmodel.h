#pragma once
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>
#include <boost/serialization/string.hpp>
#include <math/Vector4T.h>

namespace sa {
class MaterialModel
{
public:
  enum BlendMode {
    None,
    Default,
    Additive,
    Force32Bit
  };

  enum TextureMappingMode {
    Repeat         = 0x2901, // GL_REPEAT
    MirroredRepeat = 0x8370, // GL_MIRRORED_REPEAT
    ClampToEdge    = 0x812F, // GL_CLAMP_TO_EDGE
    ClampToBorder  = 0x812D  // GL_CLAMP_TO_BORDER
  };
  ~MaterialModel();
  MaterialModel();
  MaterialModel(
      bool isTwoSided,
      BlendMode blendMode,
      const char* texDirAmbient,
      const char* texDirDiffuse,
      const char* texDirSpecular,
      TextureMappingMode mappingModeAmbient,
      TextureMappingMode mappingModeDiffuse,
      TextureMappingMode mappingModeSpecular,
      const Vector4T<float> ambient,
      const Vector4T<float> diffuse,
      const Vector4T<float> specular,
      float shininess,
      float shininessStrength
      );


  bool isTwoSided() const { return m_isTwoSided; }

  BlendMode blendMode() const;

  const std::string& texDirAmbient() const { return m_texDirAmbient; }
  const std::string& texDirDiffuse() const { return m_texDirDiffuse; }
  const std::string& texDirSpecular() const { return m_texDirSpecular; }

  TextureMappingMode mappingModeAmbient() const;
  TextureMappingMode mappingModeDiffuse() const;
  TextureMappingMode mappingModeSpecular() const;

  const Vector4T<float> ambient() const { return m_ambient; }
  const Vector4T<float> diffuse() const { return m_diffuse; }
  const Vector4T<float> specular() const { return m_specular; }
  float shininess() const { return m_shininess; }
  float shininessStrength() const { return m_shininessStrength; }



private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int )
  {
    ar & BOOST_SERIALIZATION_NVP(m_isTwoSided);
    ar & BOOST_SERIALIZATION_NVP(m_blendMode);
    ar & BOOST_SERIALIZATION_NVP(m_texDirAmbient);
    ar & BOOST_SERIALIZATION_NVP(m_texDirDiffuse);
    ar & BOOST_SERIALIZATION_NVP(m_texDirSpecular);

    ar & BOOST_SERIALIZATION_NVP(m_mappingModeAmbient);
    ar & BOOST_SERIALIZATION_NVP(m_mappingModeDiffuse);
    ar & BOOST_SERIALIZATION_NVP(m_mappingModeSpecular);

    ar & BOOST_SERIALIZATION_NVP(m_ambient);
    ar & BOOST_SERIALIZATION_NVP(m_diffuse);
    ar & BOOST_SERIALIZATION_NVP(m_specular);

    ar & BOOST_SERIALIZATION_NVP(m_shininess);
    ar & BOOST_SERIALIZATION_NVP(m_shininessStrength);
  }

  bool m_isTwoSided;
  BlendMode m_blendMode;
  std::string m_texDirAmbient;
  std::string m_texDirDiffuse;
  std::string m_texDirSpecular;
  TextureMappingMode m_mappingModeAmbient = TextureMappingMode::Repeat;
  TextureMappingMode m_mappingModeDiffuse = TextureMappingMode::Repeat;
  TextureMappingMode m_mappingModeSpecular = TextureMappingMode::Repeat;
  Vector4T<float> m_ambient = {0.0f,0.0f,0.0f,1.0f};
  Vector4T<float> m_diffuse = {0.0f,0.0f,0.0f,1.0f};
  Vector4T<float> m_specular = {0.0f,0.0f,0.0f,1.0f};
  float m_shininess = 0.0f;
  float m_shininessStrength = 0.0f;
};
}
