#include "mainwindow.h"
#include <QWidget>
#include "ui_mainwindow.h"
#include "glwidget.h"

#include "models/globalsettingsmodel.h"
#include "models/entities_tree_model/entityfactoryitemmodel.h"
#include "config/config.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  sa::Config config;
  config.init("sa_config.conf");

  GlobalSettingsModelPtr globalSettingsModel = GlobalSettingsModelPtr(new GlobalSettingsModel());
  entityFactoryItemModel = new sa::EntityFactoryItemModel(this, config.getParam("CLASSES_DIR"));


  ui->globalSettingsForm->setModel(globalSettingsModel);
  ui->addEntitiesForm->setModel(entityFactoryItemModel);
  ui->widget->setModel(globalSettingsModel);
}

MainWindow::~MainWindow()
{
  delete ui;
  ui = nullptr;
  delete entityFactoryItemModel;
  entityFactoryItemModel = nullptr;
}
