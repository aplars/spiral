#pragma once

#include <QAbstractItemModel>
#include <memory>
#include "entityfactoryrootnode.h"

namespace sa {
class EntityFactoryItemModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit EntityFactoryItemModel(QObject *parent, const std::string &entitiesDir);
  virtual ~EntityFactoryItemModel();
  void refresh(const std::string& entitiesDir);

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;



  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

  Qt::ItemFlags flags(const QModelIndex &index) const;

  QStringList mimeTypes() const;
  QMimeData* mimeData(const QModelIndexList &indexes) const;
private:
  EntityFactoryGroupNode* m_root;
  std::string m_entetiesDir;
};
typedef std::shared_ptr<EntityFactoryItemModel> EntityFactoryItemModelPtr;
}
