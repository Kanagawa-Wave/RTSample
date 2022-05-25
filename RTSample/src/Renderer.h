#pragma once

#include <cstdint>

#define GLM_FORCE_AVX2
#define GLM_FORCE_ALIGNED

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Ray.h"

namespace Walnut
{
	struct RenderInfo
	{
		uint32_t* pImageData = nullptr;
	};


	class Renderer
	{
	public:
		Renderer() = default;
		Renderer(
			uint32_t viewportWidth, 
			uint32_t viewportHeight,
			float focalLength = 1.f);
		~Renderer();

		void UpdateView(uint32_t viewportWidth, uint32_t viewportHeight);
		void Render(RenderInfo& renderInfo);

		[[nodiscard]] uint32_t getPixelIndex() const { return m_PixelIndex; }

	private:
		float HitSphere(const glm::vec3& center, const float radius, const Ray& r);
		glm::vec3 RayColor(const Ray& r);

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		float m_CameraViewWidth = 0.f, m_CameraViewHeight = 0.f;

		uint32_t m_PixelIndex = 0;

		float m_FocalLength = 1.f, m_AspectRatio = 0.f;

		glm::vec3 m_Origin{}, m_Horizontal{}, m_Vertical{}, m_LowerLeftCorner{};
	};
}
