#pragma once

#include <QWidget>
#include <adt/subject.h>
#include "models/globalsettingsmodel.h"

namespace Ui {
class GlobalSettingsForm;
}

class GlobalSettingsForm : public QWidget
{
  Q_OBJECT

public:
  explicit GlobalSettingsForm(QWidget *parent = 0);
  ~GlobalSettingsForm();

  void setModel(GlobalSettingsModelPtr model);

  void updateUi();
private:
  Ui::GlobalSettingsForm *ui;
  GlobalSettingsModelPtr m_model;
};
