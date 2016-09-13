#pragma once

#include <iostream>
#include <cmath>
#include <glm/vec3.hpp>
#include "ScalarT.h"
namespace sa
{
	template <typename T>
	class Vector3T
	{
	public:
		T xyz[3];
		
		void Set(const T& x, const T& y, const T& z)
		{
			xyz[0] = x;
			xyz[1] = y;
			xyz[2] = z;

		}

		void X(const T& x) { xyz[0] = x; }		
		T X() const { return xyz[0]; }		
		void Y(const T& y) { xyz[1] = y; }		
		T Y() const { return xyz[1]; }		
		void Z(const T& z) { xyz[2] = z; }		
		T Z() const { return xyz[2]; }		
		
		Vector3T();
		Vector3T(const T& x, const T& y, const T& z);

		T DotProduct(const Vector3T& other) const;
		static T DotProduct(const Vector3T& a, const Vector3T& b);

		Vector3T CrossProduct(const Vector3T& other) const;
		static Vector3T CrossProduct(const Vector3T& a, const Vector3T& b);

		T GetNorm2() const { return xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]; }

		T GetNorm() const { return sqrt(GetNorm2()); }
		
		const Vector3T& Normalize(); 
		Vector3T GetNormalized() const; 

		static  Vector3T<T> Lerp(const T& t, Vector3T<T>& a, Vector3T<T>& b)
		{
			return (a*(static_cast<T>(1.0) - t) + b*t);
		}

		const Vector3T operator*(const T& scalar) const { return Vector3T(xyz[0] * scalar, xyz[1] * scalar, xyz[2] * scalar); }
		Vector3T& operator*=(const T& scalar)  { xyz[0]*=scalar; xyz[1]*=scalar; xyz[2]*=scalar; return *this; }
		const Vector3T operator/(const T& scalar) const { return Vector3T(xyz[0] / scalar, xyz[1] / scalar, xyz[2] / scalar); }
		Vector3T& operator/=(const T& scalar)  { xyz[0]/=scalar; xyz[1]/=scalar; xyz[2]/=scalar; return *this; }
		const Vector3T operator+(const Vector3T& other) const { return Vector3T(xyz[0] + other.xyz[0], xyz[1] + other.xyz[1], xyz[2] + other.xyz[2]); }
		Vector3T& operator+=(const Vector3T& other)  { xyz[0]+=other.xyz[0]; xyz[1]+=other.xyz[1]; xyz[2]+=other.xyz[2]; return *this; }
		const Vector3T operator-(const Vector3T& other) const { return Vector3T(xyz[0] - other.xyz[0], xyz[1] - other.xyz[1], xyz[2] - other.xyz[2]); }
		Vector3T& operator-=(const Vector3T& other)  { xyz[0]-=other.xyz[0]; xyz[1]-=other.xyz[1]; xyz[2]-=other.xyz[2]; return *this; }
    const Vector3T operator-(const T& other) const { return Vector3T(xyz[0] - other, xyz[1] - other, xyz[2] - other); }
    Vector3T& operator-=(const T& other)  { xyz[0]-=other; xyz[1]-=other; xyz[2]-=other; return *this; }
    const Vector3T operator+(const T& other) const { return Vector3T(xyz[0] + other, xyz[1] + other, xyz[2] + other); }
    Vector3T& operator+=(const T& other)  { xyz[0]+=other; xyz[1]+=other; xyz[2]+=other; return *this; }

    const Vector3T operator-() const { return Vector3T() - *this; }
		const Vector3T operator+() const { return *this; }

		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;

		const T* GetConstPtr() const { return &xyz[0]; } 
		//_E E;

		template <typename U>
		operator Vector3T<U>() const
		{
			Vector3T<U> v(static_cast<U>(xyz[0]), static_cast<U>(xyz[1]), static_cast<U>(xyz[2]));
			return v;
		}


	private:
	};

	template <typename T>
	const Vector3T<T> operator*(const T& scalar, const Vector3T<T>& v)  
	{ 
		return Vector3T<T>(v.xyz[0] * scalar, v.xyz[1] * scalar, v.xyz[2] * scalar); 
	}
	
	template <typename T>
	std::ostream& operator<<(std::ostream& output, const Vector3T<T>& v) {
		output << "(" <<  v[0] << ", " << v[1] << ", " << v[2] << ")";
		return output;  // for multiple << operators.
	}




	#include "Vector3T.inl"
}
