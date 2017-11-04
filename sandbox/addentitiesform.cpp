#include "addentitiesform.h"
#include "ui_addentitiesform.h"
#include "models/entities_tree_model/entityfactoryitemmodel.h"

AddEntitiesForm::AddEntitiesForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::AddEntitiesForm)
{
  ui->setupUi(this);
}

AddEntitiesForm::~AddEntitiesForm()
{
  delete ui;
}

void AddEntitiesForm::setModel(sa::EntityFactoryItemModel *model)
{
  ui->treeView->setModel(model);
}
