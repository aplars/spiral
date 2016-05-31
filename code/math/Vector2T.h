#pragma once

#if _DEBUG
#include "Exceptions.h"
#endif

namespace sa
{
	template <typename T>
	class Vector2T
	{
	public:
		T xy[2];

		Vector2T() 
		{ 
			xy[0] = T();
			xy[1] = T();		
		}

		Vector2T(const T& x, const T& y)
		{
			xy[0] = x;
			xy[1] = y;
		}

		void Set(const T& x, const T& y) { xy[0] = x; xy[1] = y; }

    T X() const {
      return xy[0];
    }

    T Y() const {
      return xy[1];
    }

		const Vector2T operator/(const T& scalar) const { return Vector2T(xy[0] / scalar, xy[1] / scalar); }
		Vector2T& operator/=(const T& scalar)  { xy[0]/=scalar; xy[1]/=scalar; return *this; }
		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;

    Vector2T& operator+=(const Vector2T& other)  { xy[0]+=other.xy[0]; xy[1]+=other.xy[1];  return *this; }
    const Vector2T operator-(const Vector2T& other) const { return Vector2T(xy[0] - other.xy[0], xy[1] - other.xy[1]); }
    Vector2T& operator-=(const Vector2T& other)  { xy[0]-=other.xy[0]; xy[1]-=other.xy[1]; return *this; }
    const Vector2T operator-(const T& other) const { return Vector2T(xy[0] - other, xy[1] - other); }
    Vector2T& operator-=(const T& other)  { xy[0]-=other; xy[1]-=other;  return *this; }
    const Vector2T operator+(const T& other) const { return Vector2T(xy[0] + other, xy[1] + other); }
    Vector2T& operator+=(const T& other)  { xy[0]+=other; xy[1]+=other; return *this; }

	private:

	};

	#include "Vector2T.inl"
}
