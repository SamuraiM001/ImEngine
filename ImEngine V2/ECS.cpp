#include "ECS.h"
#include <raymath.h>

using namespace IE;

template<typename T, typename ...Args>
 T* Object::AddComponent(Args && ...args) {
    std::unique_ptr<T> comp = std::make_unique<T>(std::forward<Args>(args)...);
    comp->SetOwner(this);
    T* rawPtr = comp.get();
    m_Components[typeid(T)] = std::move(comp);
    return rawPtr;
}


template<typename T>
 T* Object::GetComponent() {
    auto it = m_Components.find(typeid(T));
    if (it != m_Components.end()) {
        return dynamic_cast<T*>(it->second.get());
    }
    return nullptr;
}



 // Check if a component of a given type is attached.
bool Object::HasComponent(const std::type_info& type) const {
     return m_Components.find(type) != m_Components.end();
 }

 void Object::Update() {
    for (auto& [type, comp] : m_Components) {
        comp->Update();
    }
}

void Object::EditorUpdate(){
    for (auto& [type, comp] : m_Components) {
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

Matrix Object::GetWorldTransform()
{
    if (m_Parent)
        return m_Parent->GetWorldTransform() * GetLocalTransform(); // Local then parent
    else
        return GetLocalTransform();
}

Matrix Object::GetLocalTransform()
{
    // Create a scale matrix
    Matrix scaleMatrix = MatrixScale(m_Scale.x, m_Scale.y, m_Scale.z);

    // Create rotation matrices from Euler angles
    Matrix rotX = MatrixRotateX(m_Rotation.x);
    Matrix rotY = MatrixRotateY(m_Rotation.y);
    Matrix rotZ = MatrixRotateZ(m_Rotation.z);
    Matrix rotationMatrix = MatrixMultiply(MatrixMultiply(rotZ, rotY), rotX);

    // Create a translation matrix
    Matrix translationMatrix = MatrixTranslate(m_Position.x, m_Position.y, m_Position.z);

    // Final local transform: Scale → Rotate → Translate
    Matrix localTransform = MatrixMultiply(MatrixMultiply(scaleMatrix, rotationMatrix), translationMatrix);
    return localTransform;
}
