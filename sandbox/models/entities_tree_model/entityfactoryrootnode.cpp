#include "entityfactoryrootnode.h"
#include <QDir>
#include <QDebug>
#include "entityfactoryleafnode.h"
#include "entityfactorymodel.h"
#include <json.hpp>
#include <fstream>
#include <istream>
namespace sa {
EntityFactoryRootNode::EntityFactoryRootNode(const std::string &dir, const std::string &name)
  : EntityFactoryGroupNode(dir, name)
{ }

EntityFactoryGroupNode *EntityFactoryRootNode::loadFromDisk(const std::string &entitiesDir)
{
  QDir dir(entitiesDir.c_str());

  EntityFactoryGroupNode* groupNode = new EntityFactoryGroupNode(entitiesDir, dir.dirName().toUtf8().constData());
  for(QFileInfo fi : dir.entryInfoList(QDir::Files)) {
    if(fi.isFile() && fi.suffix() == "json") {

      EntityFactoryModel* model = loadJson(fi.absolutePath().toUtf8().constData(), fi.fileName().toUtf8().constData());
      EntityFactoryLeafNode* leafNode = new EntityFactoryLeafNode(fi.absolutePath().toUtf8().constData(), fi.fileName().toUtf8().constData());

      leafNode->setModel(model);

      groupNode->addChild(leafNode);
    }
  }
  for(QFileInfo fi : dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
    if(fi.isDir()) {
      EntityFactoryGroupNode* subNode = loadFromDisk(fi.absoluteFilePath().toUtf8().constData());
      groupNode->addChild(subNode);
    }
  }
  return groupNode;
}

EntityFactoryModel* EntityFactoryRootNode::loadJson(const std::string &dir, const std::string &file)
{
  EntityFactoryModel* model = new EntityFactoryModel(dir);
  nlohmann::json json;
  std::ifstream thestream((dir + "/" + file).c_str());
  json = json.parse(thestream);
  model->fromJson(json);
  return model;
}
}
