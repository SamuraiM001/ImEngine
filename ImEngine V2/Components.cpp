#include "Components.h"

using namespace IE;


void IE::RenderComponent::GuiRender(){
    ImGui::Button("SS");
}

void RenderComponent::Render() {
    DrawCube(GetOwner()->m_Position, 1, 1, 1, WHITE);
}
