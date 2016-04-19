#pragma once
#include "Vector3T.h"
#include "ScalarT.h"

namespace sa
{
	template <typename T>
	class PlaneT
	{
	public:
		enum Side
		{
			Front_Side,
			Back_Side,
			No_Side
		};
    PlaneT() {}
		PlaneT(const T& a, const T& b, const T& c, const T& d)
			: m_normal(a, b, c)
			, m_d(d) {} 
		PlaneT(const Vector3T<T>& n, const T& d)
			: m_normal(n)
			, m_d(d) {} 
		PlaneT(const Vector3T<T>& n, const Vector3T<T>& pnt)
			: m_normal(n)
			, m_d(-n.DotProduct(pnt)) {} 
		//TODO: ad constructor with three points.



		const Vector3T<T>& GetNormal() const { return m_normal; }
    const Vector3T<T>& normal() const { return m_normal; }
    const T& GetD() const { return m_d; }
    const T& d() const { return m_d; }

		T GetDistance(Vector3T<T>& point) const { return m_normal.DotProduct(point) + m_d; }
		Side GetSide(Vector3T<T>& point, const double& tolerance = 0.0) const;

		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;

	private:
		Vector3T<T> m_normal;
		T m_d;
	};
#include "PlaneT.inl"
}
