#include <fstream>
#include <iomanip>
#include <sstream>
#include "ImEngine.h"

using namespace IE;

void SaveManager::SaveSceneToAFile(Scene* scene) {
    std::ofstream out(scene->GetFilePath());
    if (!out.is_open()) {
        IE_LOG_ERROR("Failed to save scene to file: " + scene->GetFilePath());
        return;
    }

    out << "Scene \"" << scene->GetName() << "\"\n";

    for (const auto& [id, entity] : scene->GetEntities()) {
        out << "Entity " << "\n";
        out << "  Name \"" << entity->m_Name << "\"\n";

        out << std::fixed << std::setprecision(3);
        out << "  Position " << entity->m_Position.x << " " << entity->m_Position.y << " " << entity->m_Position.z << "\n";
        out << "  Rotation " << entity->m_Rotation.x << " " << entity->m_Rotation.y << " " << entity->m_Rotation.z << "\n";
        out << "  Scale " << entity->m_Scale.x << " " << entity->m_Scale.y << " " << entity->m_Scale.z << "\n";

        out << "  Components ";
        bool first = true;
        for (const auto& [type, comp] : entity->GetAllComponents()) {
            if (!first) out << ", ";
            out << type.name();
            first = false;
        }
        out << "\n";
    }

    out.close();
    IE_LOG_SUCCESS("Scene saved successfully to: " + scene->GetFilePath());
}


void SaveManager::LoadSceneFromAFile(Scene* scene, std::string filePath) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        IE_LOG_ERROR("Failed to load scene from file: " + filePath);
        return;
    }

    std::string line;
    Object* currentEntity = nullptr;
    scene->SetFilePath(filePath);
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "Scene") {
            std::string sceneName;
            iss >> std::quoted(sceneName);
            scene->SetName(sceneName);
        }
        else if (token == "Entity") {
            currentEntity = &scene->CreateEntity();
        }
        else if (token == "Name" && currentEntity) {
            std::string name;
            iss >> std::quoted(name);
            currentEntity->m_Name = name;
        }
        else if (token == "Position" && currentEntity) {
            iss >> currentEntity->m_Position.x >> currentEntity->m_Position.y >> currentEntity->m_Position.z;
        }
        else if (token == "Rotation" && currentEntity) {
            iss >> currentEntity->m_Rotation.x >> currentEntity->m_Rotation.y >> currentEntity->m_Rotation.z;
        }
        else if (token == "Scale" && currentEntity) {
            iss >> currentEntity->m_Scale.x >> currentEntity->m_Scale.y >> currentEntity->m_Scale.z;
        }
        // Components loading can be added here when each component has a serializer
    }

    in.close();
    IE_LOG_SUCCESS("Scene loaded successfully from: " + filePath);
}
