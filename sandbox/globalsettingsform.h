#ifndef GLOBALSETTINGSFORM_H
#define GLOBALSETTINGSFORM_H

#include <QWidget>
#include <adt/subject.h>

namespace Ui {
class GlobalSettingsForm;
}

class GlobalSettingsForm : public QWidget
{
  Q_OBJECT

public:
  explicit GlobalSettingsForm(QWidget *parent = 0);
  ~GlobalSettingsForm();

private:
  Ui::GlobalSettingsForm *ui;
};

#endif // GLOBALSETTINGSFORM_H
