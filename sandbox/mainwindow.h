#pragma once

#include <QMainWindow>
namespace sa {
class EntityFactoryItemModel;
}
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private:
  sa::EntityFactoryItemModel* entityFactoryItemModel;
  Ui::MainWindow *ui;
};
