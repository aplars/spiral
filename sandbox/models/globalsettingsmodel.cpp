#include "globalsettingsmodel.h"

GlobalSettingsModel::GlobalSettingsModel()
{

}

int GlobalSettingsModel::timeOfDay() const
{
  return m_timeOfDay;
}

void GlobalSettingsModel::setTimeOfDay(int timeOfDay)
{
//  timeOfDay = timeOfDay % 86400;
  if(m_timeOfDay != timeOfDay) {
    m_timeOfDay = timeOfDay;
    PropertyChanged.notify("timeOfDay");
  }
}

int GlobalSettingsModel::julianDay() const
{
  return m_julianDay;
}

void GlobalSettingsModel::setJulianDay(int julianDay)
{
  if(m_julianDay != julianDay) {
    m_julianDay = julianDay;
    PropertyChanged.notify("julianDay");
  }
}

int GlobalSettingsModel::timeScale() const
{
  return m_timeScale;
}

void GlobalSettingsModel::setTimeScale(int timeScale)
{
  if(m_timeScale != timeScale) {
    m_timeScale = timeScale;
    PropertyChanged.notify("timeScale");
  }
}

bool GlobalSettingsModel::isUpdating() const
{
  return m_isUpdating;
}

void GlobalSettingsModel::setIsUpdating(bool isUpdating)
{
  if(m_isUpdating != isUpdating) {
    m_isUpdating = isUpdating;
    PropertyChanged.notify("isUpdating");
  }
}

float GlobalSettingsModel::fogDensity() const
{
  return m_fogDensity;
}

void GlobalSettingsModel::setFogDensity(float fogDensity)
{
  if(m_fogDensity != fogDensity) {
    m_fogDensity = fogDensity;
    PropertyChanged.notify("fogDensity");
  }
}

float GlobalSettingsModel::atmosExposure() const
{
  return m_atmosExposure;
}

void GlobalSettingsModel::setAtmosExposure(float atmosExposure)
{
  if(m_atmosExposure != atmosExposure) {
    m_atmosExposure = atmosExposure;
    PropertyChanged.notify("atmosExposure");
  }
}

float GlobalSettingsModel::atmosDecay() const
{
  return m_atmosDecay;
}

void GlobalSettingsModel::setAtmosDecay(float atmosDecay)
{
  if(m_atmosDecay != atmosDecay) {
    m_atmosDecay = atmosDecay;
    PropertyChanged.notify("atmosDecay");
  }
}

float GlobalSettingsModel::atmosDensity() const
{
  return m_atmosDensity;
}

void GlobalSettingsModel::setAtmosDensity(float atmosDensity)
{
  if(m_atmosDensity != atmosDensity) {
    m_atmosDensity = atmosDensity;
    PropertyChanged.notify("atmosDensity");
  }
}

float GlobalSettingsModel::atmosWeight() const
{
  return m_atmosWeight;
}

void GlobalSettingsModel::setAtmosWeight(float atmosWeight)
{
  if(m_atmosWeight != atmosWeight) {
    m_atmosWeight = atmosWeight;
    PropertyChanged.notify("atmosWeight");
  }
}
