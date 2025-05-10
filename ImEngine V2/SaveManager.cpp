#include <fstream>
#include <iomanip>
#include <sstream>
#include "ImEngine.h"
#include <typeindex>

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
            std::unique_ptr<IE::Component> currentComponent = nullptr;
            std::type_index currentType = typeid(Component); 


            while (std::getline(in, line)) {
                // Trim whitespace
                line.erase(line.find_last_not_of(" \t") + 1);
                line.erase(0, line.find_first_not_of(" \t"));

                if (line.empty()) continue;
                if (line == "EndComponents") break;

                std::istringstream testStream(line);
                std::string testName;
                testStream >> testName;

                const auto& allComponents = IE::ComponentRegistry::Get().GetAll();
                auto it = allComponents.find(testName);

                if (it != allComponents.end()) {

                    if (currentComponent) {
                        currentComponent->SetOwner(currentEntity);
                        currentEntity->GetAllComponents()[currentType] = std::move(currentComponent);
                    }

                    currentComponent = IE::ComponentRegistry::Get().CreateComponent(testName);
                    if (!currentComponent) {
                        IE_LOG_WARN("Failed to create component: " + testName);
                        continue;
                    }
                    currentType = it->second();
                    currentComponent->SetOwner(currentEntity);  // Set first
                }
                else if (currentComponent) {
                    currentComponent->Deserialize(line);
                }
                else {
                    IE_LOG_WARN("Component data without component declaration: " + line);
                }
            }

            // Add the last component if it exists
            if (currentComponent) {
                currentComponent->SetOwner(currentEntity);
                currentEntity->GetAllComponents()[currentType] = std::move(currentComponent);
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
