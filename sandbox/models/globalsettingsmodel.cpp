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
  timeOfDay = timeOfDay % 86400;
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

bool GlobalSettingsModel::useStableShadowMapping() const
{
    return m_useStableShadowMapping;
}

void GlobalSettingsModel::setUseStableShadowMapping(bool useStableShadowMapping)
{
  if(m_useStableShadowMapping != useStableShadowMapping) {
    m_useStableShadowMapping = useStableShadowMapping;
    PropertyChanged.notify("useStableShadowMapping");
  }
}
