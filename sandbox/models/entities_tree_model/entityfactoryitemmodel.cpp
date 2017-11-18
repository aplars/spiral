#include "entityfactoryitemmodel.h"
#include "entityfactorygroupnode.h"
#include "entityfactorynode.h"
#include "entityfactorymodel.h"
#include <QIcon>
#include <QDir>
#include <QMimeData>
#include <QDebug>

namespace sa {
EntityFactoryItemModel::EntityFactoryItemModel(QObject *parent, const std::string& entitiesDir)
  : QAbstractItemModel(parent)
  , m_entetiesDir(entitiesDir)
  , m_root(new EntityFactoryRootNode("", "root"))
{
  refresh(m_entetiesDir);
}

EntityFactoryItemModel::~EntityFactoryItemModel() {
  delete m_root;
  m_root = nullptr;
}

void EntityFactoryItemModel::refresh(const std::string &entitiesDir)
{
  beginResetModel();
  delete m_root;
  m_root = nullptr;
  m_root = EntityFactoryRootNode::loadFromDisk(entitiesDir);
  endResetModel();
}

QVariant EntityFactoryItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  Q_UNUSED(section)
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return "Entities";
  return QVariant();
}

QModelIndex EntityFactoryItemModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(column)
  if (!hasIndex(row, 0, parent))
    return QModelIndex();

  EntityFactoryGroupNode *parentItem;

  if (!parent.isValid())
    parentItem = m_root;
  else
    parentItem = dynamic_cast<EntityFactoryGroupNode*>(static_cast<EntityFactoryNode*>(parent.internalPointer()));

  if(!parentItem)
    return QModelIndex();

  EntityFactoryNode *childItem = parentItem->childAt(row);
  if (childItem)
    return createIndex(row, 0, childItem);
  else
    return QModelIndex();
}

QModelIndex EntityFactoryItemModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  EntityFactoryNode *childItem = static_cast<EntityFactoryNode*>(index.internalPointer());
  EntityFactoryGroupNode *parentItem = dynamic_cast<EntityFactoryGroupNode*>(childItem->parent());

  if (parentItem == m_root)
    return QModelIndex();
  int row = std::find(
        dynamic_cast<const EntityFactoryGroupNode*>(parentItem->parent())->children().begin(),
        dynamic_cast<const EntityFactoryGroupNode*>(parentItem->parent())->children().end(), parentItem) - dynamic_cast<const EntityFactoryGroupNode*>(parentItem->parent())->children().begin();
  return createIndex(row, 0, parentItem);
}

int EntityFactoryItemModel::rowCount(const QModelIndex &parent) const
{
  EntityFactoryGroupNode *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = m_root;
  else
    parentItem = dynamic_cast<EntityFactoryGroupNode*>(static_cast<EntityFactoryNode*>(parent.internalPointer()));
  if(parentItem)
  {
    int size = parentItem->children().size();
    return size;
  }
  else
    return 0;
}

int EntityFactoryItemModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}


QVariant EntityFactoryItemModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

    EntityFactoryNode *node = static_cast<EntityFactoryNode*>(index.internalPointer());
    if(node) {
      EntityFactoryLeafNode* leaf = dynamic_cast<EntityFactoryLeafNode*>(node);
      if(leaf) {
        if(role == Qt::DecorationRole)
        {
          return leaf->icon();
        }
        else if(role == Qt::DisplayRole) {
          return QString(leaf->model()->displayName().c_str());
        }
      }
      else {
/*        if(role == Qt::DecorationRole)
        {
          QIcon icon;
          icon.addFile(QStringLiteral(":/icons/icons/folder.png"), QSize(), QIcon::Normal, QIcon::Off);
          return icon;
        }
        else */if(role == Qt::DisplayRole) {
          return QString(node->name().c_str());
        }
      }
    }
    return QVariant();
}

Qt::ItemFlags EntityFactoryItemModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;
  EntityFactoryNode *node = static_cast<EntityFactoryNode*>(index.internalPointer());
  EntityFactoryLeafNode* leaf = dynamic_cast<EntityFactoryLeafNode*>(node);
  if(leaf)
  {
    return Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  }
  else
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;

}

QStringList EntityFactoryItemModel::mimeTypes() const
{
  QStringList types;
  types << EntityFactoryModel::MimeType;
  return types;
}

QMimeData* EntityFactoryItemModel::mimeData(const QModelIndexList &indexes) const
{
  std::stringstream strstream;
  for(QModelIndex index : indexes) {
    EntityFactoryNode *node = static_cast<EntityFactoryNode*>(index.internalPointer());
    EntityFactoryLeafNode* leaf = dynamic_cast<EntityFactoryLeafNode*>(node);

    if(leaf) {
      strstream << leaf->dir().size() + 1;
      strstream << leaf->dir() + "/";

      strstream << leaf->name().size();
      strstream << leaf->name();

      QByteArray data(strstream.str().data(), static_cast<int>(strstream.str().size()));
      QMimeData* mimeData = new QMimeData();
      mimeData->setData(EntityFactoryModel::MimeType, data);
      return mimeData;
    }
  }
  return nullptr;
}
}
