#include "Components.h"

using namespace IE;


void RenderComponent::Render() {
    DrawCube(m_Owner->m_Position,1,1,1,WHITE);
}
