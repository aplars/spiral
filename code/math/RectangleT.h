#pragma once
#include <glm/vec2.hpp>
namespace sa
{
	template<typename T> 
	class RectangleT
	{
	public:
		RectangleT() {}

    RectangleT(const glm::vec2& pos, const glm::vec2& size)
			: m_position(pos), m_size(size) {}
		
		RectangleT(const T& posX, const T& posY, const T& sizeX, const T& sizeY)
			: m_position(posX, posY), m_size(sizeX, sizeY) {}

		void Set(const T& posX, const T& posY, const T& sizeX, const T& sizeY) 
		{
      m_position = glm::vec2(posX, posY);
      m_size = glm::vec2(sizeX, sizeY);
		}

    const glm::vec2& GetSize() const { return m_size; }
    const glm::vec2& GetPosition() const { return m_position; }

		T GetLeft() const { return m_position[0]; }
		T GetRight() const { return m_position[0] + m_size[0]; }

		T GetBottom() const { return m_position[1]; }
		T GetTop() const { return m_position[1] + m_size[1]; }

    glm::vec2 GetHalfSize() const { return m_size/2.0f; }

	private:
    glm::vec2 m_position;
    glm::vec2 m_size;
	};
}
