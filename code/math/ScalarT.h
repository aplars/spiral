#pragma once

#include <cmath>
#include <stdlib.h>

namespace sa
{
	template <typename T>
  T abs(const T& value)
	{
		return static_cast<T>(fabs(value));	
	}

	template <typename T>
  bool isZero(const T& value, double tolerance = 0.0)
	{
    return static_cast<T>(abs(value)) <= tolerance;
	}

	template <typename T>
  bool equals(const T& a, const T& b, double tolerance = 0.0)
	{
    return isZero(abs(a - b), tolerance);
	}

	template <typename T>
  T min(const T& a, const T& b)
	{
		if(a < b) return a;
		return b;
	}

	template <typename T>
  T max(const T& a, const T& b)
	{
		if(a > b) return a;
		return b;
	}

	template <typename T>
	T Sin(const T& a)
	{
		return sin(a);
	}


  template <typename T>
  T Cos(const T& a)
  {
    return cos(a);
  }

  template <typename T>
  T Atan2(const T& y, const T& x)
  {
    return atan2(y, x);
  }

  template <typename T>
	T Pow(const T& a, int p)
	{
		return pow(a, p);
	}

  template <typename T>
  T Sqrt(const T& a)
  {
    return static_cast<T>(sqrt(static_cast<double>(a)));
  }
	template <typename T>
    T Random()
    {
        return rand() / static_cast<double> (RAND_MAX);
    }

	template <typename T>
	T Random(const T& a, const T& b)
	{
		T rnd = Random<T>();
		return a * (1.0-rnd) + b * rnd;
	}
	
//	template <typename T>
//	T Pi();

//	namespace _internal
//	{
//		template <typename T>
//		class PreInitPiT
//		{
//			public:
//			PreInitPiT()
//			{
//				Pi<T>();
//			}

		
//			void DummyCall() {}

//			static PreInitPiT sPreInitPi;
//		};

//		template <typename T> PreInitPiT<T> PreInitPiT<T>::sPreInitPi;
//	}

	template <typename T>
  constexpr T Pi()
	{
    //_internal::PreInitPiT<T>::sPreInitPi.DummyCall();
    //static const T pi = static_cast<T>(acos( T() ) * 2.0);
    //return pi;
    return acos( T() ) * 2.0;
	}
	
	template <typename T>
	T Repeat(T from, T to, T v)
	{
		long toS = to-from;
		long vS = v-from;
		return from + vS%(toS+1);
	}

	template <typename T>
	T NearestPowerOf2(const T& n )
	{
		return (T) pow( 2, ceil( log( (double)n ) / log( 2.0 ) ) );
	}

	template <typename T>
	T DegToRad(const T& deg) {
		return (deg * Pi<T>() / static_cast<T>(180.0));
	}

  template <typename T>
  T RadToDeg(const T& rad) {
    return (rad*static_cast<T>(180.0)/Pi<T>());
  }

  template <typename T>
  T SinDeg(const T& a)
  {
    return sin(DegToRad(a));
  }

  template <typename T>
  T CosDeg(const T& a)
  {
    return cos(DegToRad(a));
  }

  template <typename T>
  T Atan2Deg(const T& y, const T& x)
  {
    return DegToRad(atan2(y, x));
  }

  template <typename T>
  T Fmod(const T& y, const T& x)
  {
    return fmod(y, x);
  }


}
