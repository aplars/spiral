#ifndef SUNPOSITION_H
#define SUNPOSITION_H


class SunPosition
{
public:
  SunPosition();
  void update(float /*dt*/);

private:
  float m_observerLongitude;
  float m_sunTheta = 0.0f;
  float m_sunPhi = 0.0f;
};

#endif // SUNPOSITION_H
