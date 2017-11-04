#include "entityfactorymodel.h"
namespace sa {
const char* EntityFactoryModel::MimeType = "EntityModel::MimeType";


EntityFactoryModel::EntityFactoryModel(QObject *parent, const std::string &pathToModelFile)
  : QObject(parent)
  , m_pathToModelFile(pathToModelFile)
{

}

EntityFactoryModel::EntityFactoryModel(const std::string &pathToModelFile)
  : QObject(nullptr)
  , m_pathToModelFile(pathToModelFile)
{

}

void EntityFactoryModel::fromJson(const nlohmann::json &json)
{
  nlohmann::json::const_iterator it;

  it = json.find("editorImage");
  if(it != json.end()) {
    setDisplayImageName(it.value());
  }
  it = json.find("editorName");
  if(it != json.end()) {
    setDisplayName(it.value());
  }

  it = json.find("texturemap");
  if(it != json.end()) {
    nlohmann::json::object_t o = it.value();
    nlohmann::json::object_t::iterator it2 = o.find("image");

    setTexture((*it2).second);
  }
}

const std::string &EntityFactoryModel::displayImageName() const
{
  return m_displayImageName;
}

void EntityFactoryModel::setDisplayImageName(const std::string &displayName)
{
  m_displayImageName = displayName;
}

const std::string &EntityFactoryModel::displayName() const
{
    return m_displayName;
}

void EntityFactoryModel::setDisplayName(const std::string &displayName)
{
    m_displayName = displayName;
}

const std::string &EntityFactoryModel::texture() const
{
  return m_texture;
}

void EntityFactoryModel::setTexture(const std::string &texture)
{
  m_texture = texture;
}

std::string EntityFactoryModel::pathToModelFile() const
{
  return m_pathToModelFile;
}
}
