#pragma once
#include <unordered_map>
#include <functional>
#include <iostream>
#include <string>
#include <memory>
#include <typeindex>
#include "ECS.h"  // Include Component base

namespace IE {

    class ComponentRegistry {
    public:
        static ComponentRegistry& Get() {
            static ComponentRegistry instance;
            return instance;
        }

        // Register native component (C++)
        template<typename T>
        void Register() {
            T temp;
            const std::string name = temp.m_Name();
            m_ComponentTypes[name] = []() -> std::type_index {
                return typeid(T);
                };
            m_Factories[name] = []() -> std::unique_ptr<Component> {
                return std::make_unique<T>();
                };
        }

        // Create native component
        std::unique_ptr<Component> CreateComponent(const std::string& name) const {
            auto it = m_Factories.find(name);
            if (it != m_Factories.end()) {
                return it->second();
            }
            return nullptr;
        }

        // Register a Lua script component 
        void RegisterScriptComponent(const std::string& name, const std::string& filePath) {
            m_ScriptComponents[name] = filePath;
            std::cout << "[ScriptRegistry] Registered Lua component: " << name
                << " -> " << filePath << "\n";
        }

        // Check if it's a script component
        bool IsScriptComponent(const std::string& name) const {
            return m_ScriptComponents.find(name) != m_ScriptComponents.end();
        }

        // Get script path
        const std::string& GetScriptComponentPath(const std::string& name) const {
            static const std::string empty = "";
            auto it = m_ScriptComponents.find(name);
            return (it != m_ScriptComponents.end()) ? it->second : empty;
        }

        const std::unordered_map<std::string, std::function<std::type_index()>>& GetAll() const {
            return m_ComponentTypes;
        }

        const std::unordered_map<std::string, std::string>& GetAllScriptComponents() const {
            return m_ScriptComponents;
        }

        static void RegisterComponents();

    private:
        ComponentRegistry() = default;
        ComponentRegistry(const ComponentRegistry&) = delete;
        ComponentRegistry& operator=(const ComponentRegistry&) = delete;

        std::unordered_map<std::string, std::function<std::type_index()>> m_ComponentTypes;
        std::unordered_map<std::string, std::function<std::unique_ptr<Component>()>> m_Factories;

        std::unordered_map<std::string, std::string> m_ScriptComponents;
    };

}
