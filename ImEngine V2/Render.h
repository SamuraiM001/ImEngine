#pragma once
#include <vector>
#include <iostream>


namespace IE {

	class RenderLayer {
	public:
		explicit RenderLayer() = default;
		virtual ~RenderLayer() { OnDetach(); };
		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnRender() {};
		virtual void OnEditorUpdate() {};
		virtual void OnUpdate() {};

	};

	class RenderStack{
	protected: 
		int LayerIdx = 0;
		std::vector<std::unique_ptr<RenderLayer>> m_Layers;
	public:
		void PushLayer(std::unique_ptr<RenderLayer> layer);
		void PushOverlay(std::unique_ptr<RenderLayer> overlay);
		
	
		template<typename T>
		T* GetLayer() {
			for (auto& layer : m_Layers) {
				if (auto typedLayer = dynamic_cast<T*>(layer.get())) {
					return typedLayer;
				}
			}
			return nullptr;
		}

		void Free();
		void Render();
		void Update();
	};

}
