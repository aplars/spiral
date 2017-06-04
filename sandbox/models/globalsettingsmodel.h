#ifndef GLOBALSETTINGSMODEL_H
#define GLOBALSETTINGSMODEL_H
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

  sa::Subject<const std::string&> PropertyChanged;
private:
  int m_timeOfDay = 16;
  int m_julianDay = 180;
  int m_timeScale = 1.0;
  float m_fogDensity = 0.0005;
  bool m_isUpdating = false;
};
typedef std::shared_ptr<GlobalSettingsModel> GlobalSettingsModelPtr;
#endif // GLOBALSETTINGSMODEL_H
