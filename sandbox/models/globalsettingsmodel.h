#pragma once
#include <adt/subject.h>

class GlobalSettingsModel
{
public:
  explicit GlobalSettingsModel();

  int timeOfDay() const;
  void setTimeOfDay(int timeOfDay);

  int julianDay() const;
  void setJulianDay(int julianDay);

  int timeScale() const;
  void setTimeScale(int timeScale);

  bool isUpdating() const;
  void setIsUpdating(bool isUpdating);

  float fogDensity() const;
  void setFogDensity(float fogDensity);

  sa::subject<const std::string&> PropertyChanged;
  float atmosExposure() const;
  void setAtmosExposure(float atmosExposure);

  float atmosDecay() const;
  void setAtmosDecay(float atmosDecay);

  float atmosDensity() const;
  void setAtmosDensity(float atmosDensity);

  float atmosWeight() const;
  void setAtmosWeight(float atmosWeight);

private:
  int m_timeOfDay = 16;
  int m_julianDay = 180;
  int m_timeScale = 1.0;
  float m_atmosExposure =  0.0034f;
  float m_atmosDecay =  1.0f;
  float m_atmosDensity = 0.84f;
  float m_atmosWeight = 5.65f;
  float m_fogDensity = 0.0005;
  bool m_isUpdating = false;
};
typedef std::shared_ptr<GlobalSettingsModel> GlobalSettingsModelPtr;

