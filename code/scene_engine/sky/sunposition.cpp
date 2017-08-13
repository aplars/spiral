//#include "sunposition.h"
//#include <cmath>

//SunPosition::SunPosition()
//{

//}

//void SunPosition::update(float /*dt*/) {
//  float hour = TimeOfDay / 3600.0f;
//  float solarTime = hour + 0.170 * sin( 4 * Pi<float>() * (JulianDay - 80) / 373 ) - 0.129 * sin( 2 * Pi<float>() * (JulianDay - 8) / 355) + 12 * (Latitude - Longitude) / Pi<float>();
//  float declination = 0.4093 * sin( 2.0f * Pi<float>() * (JulianDay - 81) / 368 );

//  m_sunTheta = Pi<float>() / 2 - asin(sin(Latitude) * sin(declination) - cos(Latitude) * cos(declination) * cos(Pi<float>() * solarTime / 12));
//  m_sunPhi   = atan(
//        -cos(declination) * sin(Pi<float>() * solarTime / 12.0f) /
//        ((cos(Latitude) * sin(declination) - sin(Latitude) * cos(declination) * cos(Pi<float>() * solarTime / 12.0f)))
//        );
//  if (m_sunPhi < 0)
//  {
//    m_sunPhi = 2 * Pi<float>() + m_sunPhi;
//  }
//}
