#include "globalsettingsform.h"
#include "ui_globalsettingsform.h"

GlobalSettingsForm::GlobalSettingsForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::GlobalSettingsForm)
{
  ui->setupUi(this);
}

GlobalSettingsForm::~GlobalSettingsForm()
{
  delete ui;
}
