#pragma once
#include "ECS.h"
#include "SaveManager.h"
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace IE {
    class Scene {
    public:
        Scene();
        ~Scene() {
            SaveManager::SaveSceneToAFile(this);
        }
        Object& CreateEntity();
        void DestroyEntity(uint32_t id);

        Object* GetEntity(uint32_t id);
        const std::unordered_map<uint32_t, std::unique_ptr<Object>>& GetEntities() const;

        std::string& GetFilePath() { return m_FilePath; };
        void SetFilePath(std::string newPath) { m_FilePath = newPath; }
        
        std::string& GetName() { return m_Name; }
        void SetName(std::string Name) { m_Name = Name; }

        void Clear() { m_Entities.clear(); m_Name = ""; m_FilePath = ""; m_NextEntityID = 1; };
    private:
        std::string m_FilePath;
        std::string m_Name;
        std::unordered_map<uint32_t, std::unique_ptr<Object>> m_Entities;
        uint32_t m_NextEntityID = 1;
    };

}
