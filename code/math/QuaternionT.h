#pragma once

#include "Matrix44T.h"

#include <glm/geometric.hpp>

namespace sa
{
  template <typename T>
  class QuaternionT
  {
  public:
    QuaternionT()
      : m_s(static_cast<T>(1.0))
    { }

    QuaternionT(const glm::vec3& v, const T& s)
      : m_s(s)
      , m_v(v) {}

    QuaternionT(const T& x, const T& y, const T& z, const T& w)
      : m_s(w)
      , m_v(x, y, z) {}


    QuaternionT GetConjugate() const
    {
      return QuaternionT(-m_v[0], -m_v[1], -m_v[2], m_s);
    }

    // normalising a quaternion works similar to a vector. This method will not do anything
    // if the quaternion is close enough to being unit-length.
    void Normalize()
    {
      T mag2 = m_s * m_s + m_v[0] * m_v[0] + m_v[1] * m_v[1] + m_v[2] * m_v[2];
      T mag = static_cast<T>(sqrt(mag2));
      m_s /= mag;
      m_v[0] /= mag;
      m_v[1] /= mag;
      m_v[2] /= mag;
    }

    // normalising a quaternion works similar to a vector. This method will not do anything
    // if the quaternion is close enough to being unit-length.
    QuaternionT GetNormalized() const
    {
      QuaternionT q = *this;
      q.Normalize();
      return q;
    }

    // Convert from Axis Angle
    void LoadFromAxisAngle(const glm::vec3 &v, const T& angleRad)
    {
      T sinAngle;
      angleRad *= 0.5f;
      glm::vec3 vn(v);
      vn = glm::normalize(vn);

      sinAngle = sin(angleRad);

      m_v[0] = (vn[0] * sinAngle);
      m_v[1] = (vn[1] * sinAngle);
      m_v[2] = (vn[2] * sinAngle);
      m_s = cos(angleRad);
    }

    static QuaternionT GetFromAxisAngle(const glm::vec3 &v, const T& angleRad)
    {
      QuaternionT<T> ret;
      ret.LoadFromAxisAngle(v, angleRad);
      return ret;
    }

    // Convert from Euler Angles
    //void LoadFromEuler(const T& pitchRad, const T& yawRad, const T& rollRad)
    void LoadFromEuler(const T& yyawRad, const T& ppitchRad, const T& rrollRad)
    {
      // Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
      // and multiply those together.
      // the calculation below does the same, just shorter

      float p = yyawRad  / 2.0;
      float y = rrollRad / 2.0;
      float r = ppitchRad / 2.0;

      float sinp = sin(p);
      float siny = sin(y);
      float sinr = sin(r);
      float cosp = cos(p);
      float cosy = cos(y);
      float cosr = cos(r);

      m_v[0] = sinr * cosp * cosy - cosr * sinp * siny;
      m_v[1] = cosr * sinp * cosy + sinr * cosp * siny;
      m_v[2] = cosr * cosp * siny - sinr * sinp * cosy;
      m_s = cosr * cosp * cosy + sinr * sinp * siny;

      Normalize();
    }

    static void GetFromEuler(const T& pitchRad, const T& yawRad, const T& rollRad)
    {
      QuaternionT ret;

      ret.LoadFromEuler(pitchRad, yawRad, rollRad);
      return ret;
    }



    T& operator[](unsigned int i)
    {
      if(i < 3) return m_v[i];
      else if(i==3) return m_s;

      #ifdef _DEBUG
        throw OutOfBoundsException("A quaternion can only be indexed between 0-3", "T& QuaternionT::operator[](unsigned int i)", __FILE__, __LINE__);
      #endif
    }

    const T& operator[](unsigned int i) const
    {
      if(i < 3) return m_v[i];
      else if(i==3) return m_s;
    }

    // Multiplying a quaternion q with a vector v applies the q-rotation to v
    const glm::vec3 operator*(const glm::vec3 &vec) const
    {
      glm::vec3 vn(vec);
      vn = glm::normalize(vn);

      QuaternionT<T> vecQuat, resQuat;
      vecQuat[0] = vn.x;
      vecQuat[1] = vn.y;
      vecQuat[2] = vn.z;
      vecQuat[3] = 0.0f;

      resQuat = vecQuat * GetConjugate();
      resQuat = *this * resQuat;

      return (glm::vec3(resQuat.x, resQuat.y, resQuat.z));
    }

    const QuaternionT<T> operator*(const T& s) const
    {
      return QuaternionT<T>(m_v*s, m_s*s);
    }

    const QuaternionT<T> operator+(const QuaternionT<T>& q) const
    {
      return QuaternionT<T>(m_v+q.m_v, m_s+q.m_s);
    }

    Matrix44T<T> GetMatrix() const
    {
      T x2 = m_v[0] * m_v[0];
      T y2 = m_v[1] * m_v[1];
      T z2 = m_v[2] * m_v[2];
      T xy = m_v[0] * m_v[1];
      T xz = m_v[0] * m_v[2];
      T yz = m_v[1] * m_v[2];
      T wx = m_s * m_v[0];
      T wy = m_s * m_v[1];
      T wz = m_s * m_v[2];

      // This calculation would be a lot more complicated for non-unit length quaternions
      // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
      //   OpenGL
      T zero = static_cast<T>(0.0);
      T one = static_cast<T>(1.0);
      T two = static_cast<T>(2.0);
      return Matrix44T<T>( one - two * (y2 + z2), two * (xy - wz), two * (xz + wy), zero,
          two * (xy + wz), one - two * (x2 + z2), two * (yz - wx), zero,
          two * (xz - wy), two * (yz + wx), one - two * (x2 + y2), zero,
          zero, zero, zero, one);
    }

  /*	Vector4T<T> Mult(const math::Vector4T<T>& v) const
    {
      return *this*v;
    }*/

    glm::vec3 Vec3Transform(const glm::vec3& in) const
    {
      return *this*in;
    }

    static  QuaternionT<T> Lerp(const T& pFactor, QuaternionT<T>& pStart, QuaternionT<T>& pEnd)
    {
      return (pStart*(static_cast<T>(1.0) - pFactor) + pEnd*pFactor).GetNormalized();
    }

    static  QuaternionT<T> SLerp(const T& pFactor, QuaternionT<T>& pStart, QuaternionT<T>& pEnd)
    {
      QuaternionT pOut;
      // calc cosine theta
      T cosom = pStart.m_v.x * pEnd.m_v.x + pStart.m_v.y * pEnd.m_v.y + pStart.m_v.z * pEnd.m_v.z + pStart.m_s * pEnd.m_s;

      // adjust signs (if necessary)
      QuaternionT end = pEnd;
      if( cosom < static_cast<T>(0.0))
      {
        cosom = -cosom;
        end.m_v[0] = -end.m_v[0];   // Reverse all signs
        end.m_v[1] = -end.m_v[1];
        end.m_v[2] = -end.m_v[2];
        end.m_s = -end.m_s;
      }

      // Calculate coefficients
      T sclp, sclq;
      if( (static_cast<T>(1.0) - cosom) > static_cast<T>(0.0001)) // 0.0001 -> some epsillon
      {
        // Standard case (slerp)
        T omega, sinom;
        omega = acos( cosom); // extract theta from dot product's cos theta
        sinom = sin( omega);
        sclp  = sin( (static_cast<T>(1.0) - pFactor) * omega) / sinom;
        sclq  = sin( pFactor * omega) / sinom;
      } else
      {
        // Very close, do linear interp (because it's faster)
        sclp = static_cast<T>(1.0) - pFactor;
        sclq = pFactor;
      }

      pOut.m_v[0] = sclp * pStart.m_v[0] + sclq * end.m_v[0];
      pOut.m_v[1] = sclp * pStart.m_v[1] + sclq * end.m_v[1];
      pOut.m_v[2] = sclp * pStart.m_v[2] + sclq * end.m_v[2];
      pOut.m_s = sclp * pStart.m_s + sclq * end.m_s;

      return pOut;
    }
  //private:

    T m_s;
    glm::vec3 m_v;

    private:

  };
}
