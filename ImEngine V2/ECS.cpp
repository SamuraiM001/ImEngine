#include "ECS.h"
#include <raymath.h>

using namespace IE;




 // Returns the up vector of a transform given its rotation matrix



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
        obj->Render();
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


