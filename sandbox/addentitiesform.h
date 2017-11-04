#ifndef ADDENTITIESFORM_H
#define ADDENTITIESFORM_H
#include <QWidget>

namespace sa {
class EntityFactoryItemModel;
}

namespace Ui {
class AddEntitiesForm;
}

class AddEntitiesForm : public QWidget
{
  Q_OBJECT

public:
  explicit AddEntitiesForm(QWidget *parent = 0);
  ~AddEntitiesForm();
  void setModel(sa::EntityFactoryItemModel *model);
private:
  Ui::AddEntitiesForm *ui;
};

#endif // ADDENTITIESFORM_H
