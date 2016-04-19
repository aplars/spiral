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


		const Vector2T operator/(const T& scalar) const { return Vector2T(xy[0] / scalar, xy[1] / scalar); }
		Vector2T& operator/=(const T& scalar)  { xy[0]/=scalar; xy[1]/=scalar; return *this; }
		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;

	private:

	};

	#include "Vector2T.inl"
}
