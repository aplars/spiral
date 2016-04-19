#pragma once
#include "Vector2T.h"

namespace sa
{
	template<typename T> 
	class RectangleT
	{
	public:
		RectangleT() {}

		RectangleT(const Vector2T<T>& pos, const Vector2T<T>& size)
			: m_position(pos), m_size(size) {}
		
		RectangleT(const T& posX, const T& posY, const T& sizeX, const T& sizeY)
			: m_position(posX, posY), m_size(sizeX, sizeY) {}

		void Set(const T& posX, const T& posY, const T& sizeX, const T& sizeY) 
		{
			m_position.Set(posX, posY);
			m_size.Set(sizeX, sizeY);
		}

		const Vector2T<T>& GetSize() const { return m_size; }
		const Vector2T<T>& GetPosition() const { return m_position; }

		T GetLeft() const { return m_position[0]; }
		T GetRight() const { return m_position[0] + m_size[0]; }

		T GetBottom() const { return m_position[1]; }
		T GetTop() const { return m_position[1] + m_size[1]; }

		Vector2T<T> GetHalfSize() const { return m_size/2; }

	private:
		Vector2T<T> m_position;
		Vector2T<T> m_size;
	};
}
