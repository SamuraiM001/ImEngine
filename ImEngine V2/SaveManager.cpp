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
        out << "Entity\n";
        out << "  Name \"" << entity->m_Name << "\"\n";
        out << std::fixed << std::setprecision(3);
        out << "  Position " << entity->m_Position.x << " " << entity->m_Position.y << " " << entity->m_Position.z << "\n";
        out << "  Rotation " << entity->m_Rotation.x << " " << entity->m_Rotation.y << " " << entity->m_Rotation.z << "\n";
        out << "  Scale " << entity->m_Scale.x << " " << entity->m_Scale.y << " " << entity->m_Scale.z << "\n";

        out << "  Components ";
        bool first = true;
        for (const auto& [_, component] : entity->GetAllComponents()) {
            std::string name = "";
            for (const auto& [m_name, getType] : IE::ComponentRegistry::Get().GetAll()) {
                std::unique_ptr<IE::Component> comp = IE::ComponentRegistry::Get().CreateComponent(m_name);

                // Compare the objects that the unique_ptrs are pointing to
                if (m_name == component->StaticName()) {
                    name = m_name;
                    break;
                }
            }

            if (name.empty()) {
                IE_LOG_WARN("Skipping unknown component during save");
                continue;
            }

            if (!first) out << ", ";
            out << name;
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
        else if (token == "Components" && currentEntity) {
            std::string componentLine = line.substr(line.find("Components") + 10);
            std::istringstream comps(componentLine);
            std::string compName;
            const auto& allComponents = IE::ComponentRegistry::Get().GetAll();

            while (std::getline(comps, compName, ',')) {
                compName.erase(std::remove(compName.begin(), compName.end(), ' '), compName.end());

                // Lookup type function
                auto it = allComponents.find(compName);
                if (it == allComponents.end()) {
                    continue;
                }

                std::type_index type = it->second(); // Call the function to get the type_index
                std::unique_ptr<IE::Component> comp = IE::ComponentRegistry::Get().CreateComponent(compName);
                if (comp) {
                    comp->SetOwner(currentEntity);
                    currentEntity->GetAllComponents()[type] = std::move(comp);
                }
            }

        }
    }

    in.close();
    IE_LOG_SUCCESS("Scene loaded successfully from: " + filePath);
}
