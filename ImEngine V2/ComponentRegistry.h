#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>
#include <typeindex>
#include "ECS.h"  // Ensure this file includes the definitions of IE::Component

namespace IE {

    class ComponentRegistry {
    public:
        // Singleton accessor
        static ComponentRegistry& Get() {
            static ComponentRegistry instance;
            return instance;
        }

        // Register a component type T with its unique name.
        template<typename T>
        void Register() {
            const std::string name = T::StaticName();
            m_ComponentTypes[name] = []() -> std::type_index {
                return typeid(T);
                };
            m_Factories[name] = []() -> std::unique_ptr<Component> {
                return std::make_unique<T>();
                };
        }

        // Create a component instance by its registered name.
        std::unique_ptr<Component> CreateComponent(const std::string& name) const {
            auto it = m_Factories.find(name);
            if (it != m_Factories.end()) {
                return it->second();
            }
            return nullptr;
        }

        // Retrieve all registered component names with a lambda returning its type.
        const std::unordered_map<std::string, std::function<std::type_index()>>& GetAll() const {
            return m_ComponentTypes;
        }

        static void RegisterComponents();
    private:
        ComponentRegistry() = default;
        ComponentRegistry(const ComponentRegistry&) = delete;
        ComponentRegistry& operator=(const ComponentRegistry&) = delete;

        std::unordered_map<std::string, std::function<std::type_index()>> m_ComponentTypes;
        std::unordered_map<std::string, std::function<std::unique_ptr<Component>()>> m_Factories;
    };

}
