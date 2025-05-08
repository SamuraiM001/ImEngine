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

        out << "  Components " << '\n';
        for (const auto& [_, component] : entity->GetAllComponents()) {

            out << "    " << component->m_Name() << '\n';
            component->Serialize(out);
        }
        out << "  EndComponents";
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
    scene->Clear();
    
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
        else if (token == "Components" && currentEntity) {
            while (std::getline(in, line)) {
                std::istringstream compStream(line);
                std::string compName;
                compStream >> compName;

                if (compName == "EndComponents")break;


                const auto& allComponents = IE::ComponentRegistry::Get().GetAll();

                auto it = allComponents.find(compName);

                if (it == allComponents.end()) {
                    continue;
                }


                std::unique_ptr<IE::Component> comp = IE::ComponentRegistry::Get().CreateComponent(compName);
                if (!comp) {
                    IE_LOG_WARN("Unknown component during deserialization: " + compName);
                    continue;
                }
                std::type_index type = it->second();

                comp->SetOwner(currentEntity);

                currentEntity->GetAllComponents()[type] = std::move(comp);
                currentEntity->GetAllComponents()[type]->Deserialize(in);
            }
        }

        else if (token == "Position" && currentEntity && currentEntity->GetComponent<TransformComponent>()) {

            iss >> currentEntity->GetComponent<TransformComponent>()->m_Position.x >> currentEntity->GetComponent<TransformComponent>()->m_Position.y >> currentEntity->GetComponent<TransformComponent>()->m_Position.z;
        }
        else if (token == "Rotation" && currentEntity && currentEntity->GetComponent<TransformComponent>()) {
            iss >> currentEntity->GetComponent<TransformComponent>()->m_Rotation.x >> currentEntity->GetComponent<TransformComponent>()->m_Rotation.y >> currentEntity->GetComponent<TransformComponent>()->m_Rotation.z;
        }
        else if (token == "Scale" && currentEntity && currentEntity->GetComponent<TransformComponent>()) {
            iss >> currentEntity->GetComponent<TransformComponent>()->m_Scale.x >> currentEntity->GetComponent<TransformComponent>()->m_Scale.y >> currentEntity->GetComponent<TransformComponent>()->m_Scale.z;
        }
    }

    in.close();
    IE_LOG_SUCCESS("Scene loaded successfully from: " + filePath);
}
