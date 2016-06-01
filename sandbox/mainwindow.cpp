#include "mainwindow.h"
#include <QWidget>
#include "ui_mainwindow.h"
#include "glwidget.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

 // GLWidget* view = new GLWidget(ui->centralWidget);
  //ui->verticalLayout->addWidget(view);
}

MainWindow::~MainWindow()
{
  delete ui;
}
