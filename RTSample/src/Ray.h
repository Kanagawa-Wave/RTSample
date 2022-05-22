#pragma once

#include <glm/vec3.hpp>

class Ray
{
public:
	Ray() = default;
	Ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin(origin), m_direction(direction) {}

	glm::vec3 origin() const { return m_origin; }
	glm::vec3 direction() const { return m_direction; }

	glm::vec3 at(float distance) const { return m_origin + m_direction*distance; }

public:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
};