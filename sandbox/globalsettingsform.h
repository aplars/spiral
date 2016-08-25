#ifndef GLOBALSETTINGSFORM_H
#define GLOBALSETTINGSFORM_H

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

  void setModel(GlobalSettingsModel *model);

  void updateUi();
private:
  Ui::GlobalSettingsForm *ui;
  GlobalSettingsModel* m_model;
};

#endif // GLOBALSETTINGSFORM_H
