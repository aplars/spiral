#include "mainwindow.h"
#include <QWidget>
#include "ui_mainwindow.h"
#include "glwidget.h"

#include "models/globalsettingsmodel.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  GlobalSettingsModel* globalSettingsModel = new GlobalSettingsModel();
  ui->globalSettingsForm->setModel(globalSettingsModel);
  ui->widget->setModel(globalSettingsModel);

}

MainWindow::~MainWindow()
{
  delete ui;
}
