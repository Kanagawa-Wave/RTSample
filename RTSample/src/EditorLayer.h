#pragma once

#include "Walnut/Application.h"

#include "Walnut/Image.h"
#include "Walnut/Layer.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

namespace Walnut
{
	class EditorLayer : public Layer
	{
	public:
		virtual void OnAttach() override;
		virtual void OnUIRender() override;
		void Render();

	private:
		std::shared_ptr<Image> m_Image;
		uint32_t* m_ImageData = nullptr;
		std::unique_ptr<Renderer> m_Renderer;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		float m_LastRenderTime = 0.f;
		uint32_t m_PixelIndex = 0;

		bool m_EnableRealtime = true;
	};
}
