#include "Render.h"

using namespace IE;

void RenderStack::PushLayer(std::unique_ptr<RenderLayer> layer) {
    m_Layers.insert(m_Layers.begin() + LayerIdx, std::move(layer));
    m_Layers[LayerIdx]->OnAttach();
    LayerIdx++;
}

void RenderStack::PushOverlay(std::unique_ptr<RenderLayer> overlay) {
    m_Layers.push_back(std::move(overlay));
    m_Layers.back()->OnAttach();
}

void RenderStack::Render() {
    for (auto& layer : m_Layers) {
        layer->OnRender();
    }
}

void RenderStack::Update(){
    for (auto& layer : m_Layers) {
        layer->OnEditorUpdate();
    }
}

void RenderStack::Free() {
    for (auto& layer : m_Layers) {
        layer->OnDetach();
    }
}