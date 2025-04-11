#pragma once
#include <vector>
#include <iostream>


namespace IE {

	class RenderLayer {
	public:
		explicit RenderLayer() = default;
		virtual ~RenderLayer() = default;  
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnRender() {};
		virtual void OnUpdate() {};
	};

	class RenderStack{
	protected: 
		int LayerIdx = 0;
		std::vector<std::unique_ptr<RenderLayer>> Layers;
	public:
		void PushLayer(std::unique_ptr<RenderLayer> layer);
		void PushOverlay(std::unique_ptr<RenderLayer> overlay);
		void Render();
		void Update();
	};

}
