#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include <math/Vector3T.h>
#include <math/Vector4T.h>

namespace sa {
class DirectionalLight
{
public:
  ~DirectionalLight();
  DirectionalLight(
      const Vector3T<float>& direction,
      const Vector4T<float>& ambient,
      const Vector4T<float>& diffuse);

  const Vector3T<float>& direction() const;
  void setDirection(const Vector3T<float> &direction);

  const Vector4T<float>& ambient() const;
  void setAmbient(const Vector4T<float> &ambient);

  const Vector4T<float>& diffuse() const;
  void setDiffuse(const Vector4T<float> &diffuse);


private:
  Vector3T<float> m_direction;
  Vector4T<float> m_ambient;
  Vector4T<float> m_diffuse;

};
}
#endif // DIRECTIONALLIGHT_H
