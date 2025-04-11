#pragma once
#include "ECS.h"
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace IE {

    class Scene {
    public:

        Object& CreateEntity();
        void DestroyEntity(uint32_t id);

        Object* GetEntity(uint32_t id);
        const std::unordered_map<uint32_t, std::unique_ptr<Object>>& GetEntities() const;

    private:
        std::unordered_map<uint32_t, std::unique_ptr<Object>> m_Entities;
        uint32_t m_NextEntityID = 1;
    };

}
