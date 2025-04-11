#include "ECS.h"

using namespace IE;

template<typename T, typename ...Args>
T* Object::AddComponent(Args && ...args) {
    static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
    auto comp = std::make_unique<T>(std::forward<Args>(args)...);
    T* ptr = comp.get();
    m_Components[typeid(T)] = std::move(comp);
    return ptr;
}

template<typename T>
T* Object::GetComponent(){
    auto it = m_Components.find(typeid(T));
    if (it != m_Components.end()) {
        return static_cast<T*>(it->second.get());
    }
    return nullptr;
}

void Object::SetTransform(Matrix Transform) {
    m_Model.transform = Transform;
}

void IE::Object::SetPositon(Vector3 Positon){
    m_Position = Positon;
}

void IE::Object::SetScale(Vector3 Scale){
    m_Scale = Scale;
}

std::unordered_map<std::type_index, std::unique_ptr<Component>>& Object::GetAllComponents() {
    return m_Components;
}
