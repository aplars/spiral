#include "materialmodel.h"
namespace sa {
MaterialModel::~MaterialModel()
{

}
MaterialModel::MaterialModel()
{ }

MaterialModel::MaterialModel(
    bool isTwoSided,
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
    float shininessStrength)
  : m_isTwoSided(isTwoSided)
  , m_texDirAmbient(texDirAmbient)
  , m_texDirDiffuse(texDirDiffuse)
  , m_texDirSpecular(texDirSpecular)
  , m_mappingModeAmbient(mappingModeAmbient)
  , m_mappingModeDiffuse(mappingModeDiffuse)
  , m_mappingModeSpecular(mappingModeSpecular)
  , m_ambient(ambient)
  , m_diffuse(diffuse)
  , m_specular(specular)
  , m_shininess(shininess)
  , m_shininessStrength(shininessStrength)
{

}


MaterialModel::TextureMappingMode MaterialModel::mappingModeAmbient() const
{
  return m_mappingModeAmbient;
}

MaterialModel::TextureMappingMode MaterialModel::mappingModeDiffuse() const
{
  return m_mappingModeDiffuse;
}

MaterialModel::TextureMappingMode MaterialModel::mappingModeSpecular() const
{
  return m_mappingModeSpecular;
}
}
