#pragma once
#include <string>
#include <json.hpp>
#include <glm/vec3.hpp>
#include <QObject>
namespace sa {
class EntityFactoryModel : public QObject
{
  Q_OBJECT
public:
  static const char* MimeType;

  EntityFactoryModel(QObject* parent, const std::string& pathToModelFile);

  EntityFactoryModel(const std::string& pathToModelFile);

  void fromJson(const nlohmann::json& json);

  const std::string& displayImageName() const;
  void setDisplayImageName(const std::string &displayImageName);

  const std::string& displayName() const;
  void setDisplayName(const std::string &displayName);

  const std::string& texture() const;
  void setTexture(const std::string &texture);

  std::string pathToModelFile() const;

private:
  std::string m_pathToModelFile;
  std::string m_displayName;
  std::string m_displayImageName;
  std::string m_texture;
signals:
};
}

