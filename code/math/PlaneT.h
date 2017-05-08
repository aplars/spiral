#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


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
      : m_normal(glm::normalize(glm::vec3(a, b, c)))
			, m_d(d) {} 
    PlaneT(const glm::vec3& n, const T& d)
      : m_normal(glm::normalize(n))
			, m_d(d) {} 
    PlaneT(const glm::vec3& n, const glm::vec3& pnt)
      : m_normal(glm::normalize(n))
      , m_d(-glm::dot(m_normal, pnt)) {}

    const glm::vec3& normal() const { return m_normal; }
    const T& d() const { return m_d; }

    T distance(glm::vec3& point) const { return glm::dot(m_normal, point) + m_d; }
    Side side(glm::vec3& point, const double& tolerance = 0.0) const;

		T& operator[](unsigned int i);
		const T& operator[](unsigned int i) const;

	private:
    glm::vec3 m_normal;
		T m_d;
	};
#include "PlaneT.inl"
}
