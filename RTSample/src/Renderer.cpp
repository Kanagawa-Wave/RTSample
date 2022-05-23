#include "Renderer.h"

#include <glm/geometric.hpp>

#include "Ray.h"
#include "Utils.h"

namespace Walnut
{
	Renderer::Renderer(uint32_t viewportWidth, uint32_t viewportHeight, float focalLength)
		: m_FocalLength(focalLength)
	{
		UpdateView(viewportWidth, viewportHeight);
	}

	Renderer::~Renderer() = default;

	void Renderer::UpdateView(uint32_t viewportWidth, uint32_t viewportHeight)
	{
		m_ViewportWidth = viewportWidth;
		m_ViewportHeight = viewportHeight;
		m_AspectRatio = 
			static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
		m_CameraViewHeight = 2.f;
		m_CameraViewWidth = m_AspectRatio * m_CameraViewHeight;
		m_Origin = glm::vec3(0, 0, 0);
		m_Horizontal = glm::vec3(m_CameraViewWidth, 0, 0);
		m_Vertical = glm::vec3(0, m_CameraViewHeight, 0);
		m_LowerLeftCorner = 
			m_Origin - 
			m_Horizontal / 2.f - 
			m_Vertical / 2.f - 
			glm::vec3(0, 0, m_FocalLength);
	}

	void Renderer::Render(RenderInfo& renderInfo)
	{
		m_PixelIndex = 0;
		for (int column = m_ViewportHeight - 1; column >= 0; --column)
	    {
	        for (uint32_t row = 0; row < m_ViewportWidth; ++row)
	        {
		        const float u = static_cast<float>(row) / static_cast<float>(m_ViewportWidth - 1);
		        const float v = static_cast<float>(column) / static_cast<float>(m_ViewportHeight - 1);
		        Ray r(m_Origin, m_LowerLeftCorner + u * m_Horizontal + v * m_Vertical - m_Origin);
		        const glm::vec3 pixelColor = RayColor(r) * 255.f;
		        uint32_t tp = RGB2HEX(pixelColor);
				renderInfo.pImageData[m_PixelIndex] = tp;
				
				m_PixelIndex++;
	        }
	    }
	}

	float Renderer::HitSphere(const glm::vec3& center, const float radius, const Ray& r)
	{
	    const glm::vec3 oc = r.origin() - center;
	    const float a = glm::dot(r.direction(), r.direction());
	    const float b = 2.f * glm::dot(oc, r.direction());
	    const float c = glm::dot(oc, oc) - radius * radius;
	    const float discriminant = b * b - 4.f * a * c;
	    if (discriminant < 0.f)
	    {
	        return -1.f;
	    }
	    else
	    {
	        return (-b - sqrt(discriminant)) / (2.f * a);
	    }
	}

	glm::vec3 Renderer::RayColor(const Ray& r)
	{
	    float t = HitSphere(glm::vec3(0.f, 0.f, -1.f), 0.5f, r);
	    if (t > 0.f)
	    {
		    glm::vec3 N = glm::normalize(r.at(t) - glm::vec3(0, 0, -1));
	        return 0.5f * glm::vec3(N.x + 1.f, N.y + 1.f, N.z + 1.f);
	    }

	    const glm::vec3 unitDirection = glm::normalize(r.direction());
	    t = 0.5f * (unitDirection.y + 1.f);

	    // t = 1, outputs blue
	    // t = 0, outputs white
	    return (1.f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
	}
	
}
