#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>
#include <raylib.h> 

namespace IE {

    class Object;

    class Component {
    private:
        Object* m_Owner = nullptr;
        std::string m_Name;
    public:
        virtual ~Component() = default;
        virtual void Start() {}
        virtual void Update() {}
        virtual void EditorUpdate() {}
        virtual void Render() {}
        virtual void GuiRender() {};

        static std::string StaticName() { return "RenderComponent"; }
       
        void SetOwner(Object* owner) { m_Owner = owner; }
        Object* GetOwner() const { return m_Owner; }
        std::string GetName() const { return m_Name; }
    };

    class Object {
    public:
        Object(uint32_t id) : m_ID(id) {}

        // Template to add a component by constructing it with given arguments.
        template<typename T, typename... Args>
        T* AddComponent(Args&&... args);

        // Template to retrieve a component by type.
        template<typename T>
        T* GetComponent();

        // Check if a component of a given type is attached.
        bool HasComponent(const std::type_info& type) const {
            return m_Components.find(type) != m_Components.end();
        }

        std::unordered_map<std::type_index, std::unique_ptr<Component>>& GetAllComponents() {
            return m_Components;
        }

        void Update();
        void EditorUpdate();
        
        void AddChild(std::shared_ptr<Object> child);


        void Render();

        // Setters 
        void SetPosition(Vector3 position) { m_Position = position; }
        void SetScale(Vector3 scale) { m_Scale = scale; }

        uint32_t GetID() const { return m_ID; }
        bool isSelected = false;
        void Select() { isSelected = true; }
        void UnSelect() { isSelected = false; }

    public:
        std::string m_Name;
        Vector3 m_Position = { 0, 0, 0 };
        Vector3 m_Rotation = { 0, 0, 0 };
        Vector3 m_Scale = { 1, 1, 1 };
    private:
        uint32_t m_ID;
        std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
        
        Object* m_Parent = nullptr;
        std::vector<std::shared_ptr<Object>> m_Children;
    };


    // Template to add a component by constructing it with given arguments.



}
