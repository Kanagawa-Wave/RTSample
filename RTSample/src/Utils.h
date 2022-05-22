#pragma once


// input (0~255, 0~255, 0~255)
inline uint32_t RGB2HEX(glm::vec3 color)
{
	return (
		0 << 32 |
		255 << 24 |
		static_cast<uint32_t>(color.b) << 16 |
		static_cast<uint32_t>(color.g) << 8 |
		static_cast<uint32_t>(color.r)
		);
}