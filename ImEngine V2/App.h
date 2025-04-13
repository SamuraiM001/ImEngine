#pragma once
#include "Render.h"


namespace IE {

	class App {
	protected:
		RenderStack m_rStack;
	public:
		virtual void Initialize(int argc, char* argv[]) {};
		void PushLayer(std::unique_ptr<RenderLayer> layer) {
			m_rStack.PushLayer(std::move(layer));
		}
		void PushOverlay(std::unique_ptr<RenderLayer> layer) {
			m_rStack.PushOverlay(std::move(layer));
		}

		virtual void Run() {};
		virtual void Shutdown() {};
	};

}

