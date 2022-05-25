#include "EditorLayer.h"


namespace Walnut
{
	void EditorLayer::OnAttach()
	{
		m_Renderer = std::make_unique<Renderer>();
	}

	void EditorLayer::OnUIRender()
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		ImGui::Text("Viewport: %d * %d", m_ViewportWidth, m_ViewportHeight);
		ImGui::Text("Pixels rendered: %d", m_Renderer->getPixelIndex());
		ImGui::Text("Pixel filling rate: %.3f pixel/ms", m_Renderer->getPixelIndex() / m_LastRenderTime);
		ImGui::Checkbox("Realtime", &m_EnableRealtime);
		if (ImGui::Button("Render"))
		{
			Render();
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = static_cast<uint32_t>(ImGui::GetContentRegionAvail().x);
		m_ViewportHeight = static_cast<uint32_t>(ImGui::GetContentRegionAvail().y);
		m_Renderer->UpdateView(m_ViewportWidth, m_ViewportHeight);

		if (m_Image)
			ImGui::Image(m_Image->GetDescriptorSet(), { (float)m_Image->GetWidth(), (float)m_Image->GetHeight() });

		ImGui::End();
		ImGui::PopStyleVar();

		if (m_EnableRealtime)
			Render();
	}

	void EditorLayer::Render()
	{
		Timer timer;

		if (!m_Image || m_ViewportWidth != m_Image->GetWidth() || m_ViewportHeight != m_Image->GetHeight())
		{
			m_Image = std::make_shared<Image>(m_ViewportWidth, m_ViewportHeight, ImageFormat::RGBA);
			delete[] m_ImageData;
			m_ImageData = new uint32_t[m_ViewportWidth * m_ViewportHeight];
		}

		RenderInfo renderInfo{};
		renderInfo.pImageData = m_ImageData;
		m_Renderer->Render(renderInfo);

		m_Image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}

}
