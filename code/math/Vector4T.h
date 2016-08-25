#pragma once
#include "Vector3T.h"

namespace sa
{
	template <typename T>
	class Vector4T
	{
	public:
		Vector4T();
		Vector4T(T x, T y, T z, T w);
		Vector4T(const Vector3T<T>& v3, T w) {
			E[0] = v3[0];
			E[1] = v3[1];
			E[2] = v3[2];
			E[3] = w;
		}

		void Set(const T& x, const T& y, const T& z, const T& w)
		{
			 E[0] = x;
			 E[1] = y;
			 E[2] = z;
			 E[3] = w;
		}

		T DotProduct(const Vector4T<T>& other) const;
		static T DotProduct(const Vector4T<T>& a, const Vector4T<T>& b);

		T GetX() const;
		T GetY() const;
		T GetZ() const;
		T GetW() const;

		Vector3T<T> XYZ() const { return Vector3T<T>(E[0], E[1], E[2]); }

		T& operator[](unsigned int i);

		const T& operator[](unsigned int i) const;

		const Vector4T<T>& operator+=(const Vector4T<T>& other) {
			E[0] += other.E[0];
			E[1] += other.E[1];
			E[2] += other.E[2];
			E[3] += other.E[3];
			return *this;
		}

    static Vector4T Min(const Vector4T& a, const Vector4T& b) {
      Vector4T c;
      for(unsigned int i = 0; i < 4; ++i) {
        c[i] = Min(a[i], b[i]);
      }
      return c;
    }
    static Vector4T Max(const Vector4T& a, const Vector4T& b) {
      Vector4T c;
      for(unsigned int i = 0; i < 4; ++i) {
        c[i] = Max(a[i], b[i]);
      }
      return c;
    }


		const T* GetConstPtr() const { return &E[0]; }

		T E[4];
	private:
	};
}


#include "Vector4T.inl"
