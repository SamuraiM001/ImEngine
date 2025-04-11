#include "Render.h"

using namespace IE;

void RenderStack::PushLayer(std::unique_ptr<RenderLayer> layer) {
    Layers.insert(Layers.begin() + LayerIdx, std::move(layer));
    Layers[LayerIdx]->OnAttach();
    LayerIdx++;
}

void RenderStack::PushOverlay(std::unique_ptr<RenderLayer> overlay) {
    Layers.push_back(std::move(overlay));
    Layers.back()->OnAttach();
}

void RenderStack::Render() {
    for (auto& layer : Layers) {
        layer->OnRender();
    }
}

void IE::RenderStack::Update(){
    for (auto& layer : Layers) {
        layer->OnUpdate();
    }
}
