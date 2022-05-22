#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

#include "Ray.h"
#include "Util.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		ImGui::Text("Viewport: %d * %d", m_ViewportWidth, m_ViewportHeight);
		ImGui::Text("Pixels rendered: %d", m_PixelIndex);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);
		m_AspectRatio = (float)m_ViewportWidth / (float)m_ViewportHeight;
		m_CameraViewHeight = 2.f;
		m_CameraViewWidth = m_AspectRatio * m_CameraViewHeight;

		if (m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void Render()
	{
		Timer timer;

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		constexpr auto origin = glm::vec3(0, 0, 0);
		const auto horizontal = glm::vec3(m_CameraViewWidth, 0, 0);
		const auto vertical = glm::vec3(0, m_CameraViewHeight, 0);
		const auto lowerLeftCorner = origin - horizontal / 2.f - vertical / 2.f - glm::vec3(0, 0, m_FocalLength);

		m_PixelIndex = 0;
		for (int column = m_ViewportHeight - 1; column >= 0; --column)
	    {
	        for (int row = 0; row < m_ViewportWidth; ++row)
	        {
		        const float u = static_cast<float>(row) / static_cast<float>(m_ViewportWidth - 1);
		        const float v = static_cast<float>(column) / static_cast<float>(m_ViewportHeight - 1);
		        Ray r(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
		        const glm::vec3 pixelColor = RayColor(r) * 255.f;
		        uint32_t tp = rgb2hex(pixelColor);
				m_ImageData[m_PixelIndex] = tp;
				
				m_PixelIndex++;
				
	        }
	    }

		m_Image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}

	float HitSphere(const glm::vec3& center, const float radius, const Ray& r)
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

	glm::vec3 RayColor(const Ray& r)
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
private:
	std::shared_ptr<Image> m_Image;
	uint32_t* m_ImageData = nullptr;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	float m_CameraViewWidth = 0.f, m_CameraViewHeight = 0.f;

	float m_LastRenderTime = 0.0f;
	uint32_t m_PixelIndex = 0;

	float m_FocalLength = 1.f, m_AspectRatio = 0.f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}