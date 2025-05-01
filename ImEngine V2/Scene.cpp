#include "Scene.h"

using namespace IE;

Scene::Scene() {
}


Object& Scene::CreateEntity() {
    uint32_t id = m_NextEntityID++;
    auto object = std::make_unique<Object>(id);
    object->m_Name = "NewObject" + std::to_string(id);
    object->SetScene(this);
    Object& ref = *object;
    m_Entities[id] = std::move(object);
    return ref;
}

void Scene::DestroyEntity(uint32_t id) {
    UnParent(id);
    m_Entities.erase(id);
}

void Scene::UnParent(uint32_t id){
    m_Entities.at(id).get()->SetParent(nullptr);
}

Object* Scene::GetEntity(uint32_t id) {
    auto it = m_Entities.find(id);
    if (it != m_Entities.end()) {
        return it->second.get();
    }
    return nullptr;
}

const std::unordered_map<uint32_t, std::shared_ptr<Object>>& Scene::GetEntities() const {
    return m_Entities;
}


