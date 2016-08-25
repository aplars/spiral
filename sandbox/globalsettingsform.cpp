#include "globalsettingsform.h"
#include "ui_globalsettingsform.h"
#include <QSpinBox>
#include <QTimer>

double dt = 1.0/60.0;

GlobalSettingsForm::GlobalSettingsForm(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::GlobalSettingsForm)
{
  ui->setupUi(this);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, [=]() {

    m_model->setTimeOfDay(m_model->timeOfDay()+dt*m_model->timeScale());
    if (m_model->timeOfDay() > 3600 * 24) //next day?
    {
      m_model->setJulianDay(m_model->julianDay() + 1.0f);
      m_model->setTimeOfDay(m_model->timeOfDay() - 3600 * 24);
    }

  });
  //timer->start(16);


  QObject::connect<void(QDoubleSpinBox::*)(double)>(ui->timeOfDaySpinBox, &QDoubleSpinBox::valueChanged, [=](double value) {
    m_model->setTimeOfDay(value);
  });
  QObject::connect<void(QDoubleSpinBox::*)(double)>(ui->julianDaySpinBox, &QDoubleSpinBox::valueChanged, [=](double value) {
    m_model->setJulianDay(value);
  });
  QObject::connect<void(QDoubleSpinBox::*)(double)>(ui->sunTimeScaleSpinBox, &QDoubleSpinBox::valueChanged, [=](double value) {
    m_model->setTimeScale(value);
  });
  QObject::connect<void(QLineEdit::*)()>(ui->atmosphereFogDensityLineEdit, &QLineEdit::editingFinished, [=]() {
    m_model->setFogDensity(ui->atmosphereFogDensityLineEdit->text().toFloat());
  });

  QObject::connect(ui->runSunButton, &QPushButton::clicked, [=]() {
    QObject::disconnect(ui->timeOfDaySpinBox);
    QObject::disconnect(ui->julianDaySpinBox);
    timer->start(16);
  });

  QObject::connect(ui->stopSunButton, &QPushButton::clicked, [=]() {
    timer->stop();
  });

  QObject::connect(ui->useStableShadowsCheckBox, &QCheckBox::toggled, [=](bool isChecked) {
    m_model->setUseStableShadowMapping(isChecked);
  });

}

GlobalSettingsForm::~GlobalSettingsForm()
{
  delete ui;
}

void GlobalSettingsForm::setModel(GlobalSettingsModel *model)
{
  m_model = model;

  m_model->PropertyChanged += [this](const std::string& /*property*/) {
    updateUi();
  };

  updateUi();
}

void GlobalSettingsForm::updateUi() {
  ui->timeOfDaySpinBox->setValue(m_model->timeOfDay());
  ui->julianDaySpinBox->setValue(m_model->julianDay());
  ui->atmosphereFogDensityLineEdit->setText(QString::number(m_model->fogDensity()));
  ui->useStableShadowsCheckBox->setChecked(m_model->useStableShadowMapping());
}
