#include "ECS.h"
#include <raymath.h>

using namespace IE;



 void Object::Update() {
    for (auto& [type, comp] : m_Components) {
        if(comp->m_isActive)
        comp->Update();
    }
}

void Object::EditorUpdate(){
    for (auto& [type, comp] : m_Components) {
        if (comp->m_isActive)
        comp->EditorUpdate();

    }
}

void Object::Start(){
    for (auto& [type, comp] : m_Components) {
        comp->Start();
    }
}

void Object::AddChild(Object* child) {
    child->m_Parent = this;
    m_Children.push_back(child);
}

void Object::SetParent(Object* newParent)
{
    if (!IsAncestorOf(newParent)) {
        if (m_Parent)
        {
            auto& siblings = m_Parent->m_Children;
            siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
        }
        m_Parent = newParent;
        if (newParent)
            newParent->m_Children.push_back(this);
    }

}

void Object::Render(){
    for (auto& [type, obj] : m_Components) {
        if (obj->m_isActive) {
            obj->Render();
        }
    }
}

void Object::RenderSelection(){
    for (auto& [type, obj] : m_Components) {
        if (obj->m_isActive)
        obj->RenderSelection();
    }
}

bool Object::IsAncestorOf(Object* pD)
{
    Object* current = pD;
    while (current)
    {
        if (current == this)
            return true;
        current = current->GetParent();
    }
    return false;
}


