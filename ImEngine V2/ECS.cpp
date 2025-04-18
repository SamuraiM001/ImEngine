#include "ECS.h"

using namespace IE;

template<typename T, typename ...Args>
inline T* Object::AddComponent(Args && ...args) {
    std::unique_ptr<T> comp = std::make_unique<T>(std::forward<Args>(args)...);
    comp->SetOwner(this);
    T* rawPtr = comp.get();
    m_Components[typeid(T)] = std::move(comp);
    return rawPtr;
}


template<typename T>
inline T* IE::Object::GetComponent() {
    auto it = m_Components.find(typeid(T));
    if (it != m_Components.end()) {
        return dynamic_cast<T*>(it->second.get());
    }
    return nullptr;
}


void Object::Update() {
    for (auto& [type, obj] : m_Components) {
        obj->Update();
    }
}

void Object::EditorUpdate(){
    for (auto& [type, obj] : m_Components) {
        obj->EditorUpdate();
    }
}

void Object::Render(){
    for (auto& [type, obj] : m_Components) {
        obj->Render();
    }
}
