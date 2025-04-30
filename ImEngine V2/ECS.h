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
    public:
        virtual std::string m_Name() { return "SA"; };
        virtual ~Component() = default;
        virtual void Start() {}
        virtual void Update() {}
        virtual void EditorUpdate() {}
        virtual void Render() {}
        virtual void GuiRender() {};

       
        void SetOwner(Object* owner) { m_Owner = owner; }
        Object* GetOwner() const { return m_Owner; }
       
    };

    class Object {
    public:
        Object(uint32_t id) : m_ID(id) , m_BillboardText(LoadTexture("textures/Logo.png")) {}

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args);

        template<typename T>
        T* GetComponent();

        bool HasComponent(const std::type_info& type) const;

        std::unordered_map<std::type_index, std::unique_ptr<Component>>& GetAllComponents() {return m_Components;}

        void Update();
        void EditorUpdate();
        
        void AddChild(Object* child);

        void SetParent(Object* newParent);
        Object* GetParent() { return m_Parent; }

        void Render();

        Texture2D* GetBillboardTexture() { return &m_BillboardText; };
   
        bool IsAncestorOf(IE::Object* potentialDescendant);


        // Setters 
        void SetPosition(Vector3 position) { m_Position = position; }
        void SetRotation(Vector3 rotation) { m_Rotation = rotation; }
        void SetScale(Vector3 scale) { m_Scale = scale; }


        Matrix GetWorldTransform();

        Matrix GetLocalTransform();


        uint32_t GetID() const { return m_ID; }
        bool isSelected = false;
        void Select() { isSelected = true; }
        void UnSelect() { isSelected = false; }
        std::vector<Object*>& GetChildren() { return m_Children; };

    public:
        std::string m_Name;
        Vector3 m_Position = { 0, 0, 0 };
        Vector3 m_Rotation = { 0, 0, 0 };
        Vector3 m_Scale = { 1, 1, 1 };
    private:
        uint32_t m_ID;
        std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
        
        Texture2D m_BillboardText;
        std::vector<Object*> m_Children;
        Object* m_Parent = nullptr;

    };


    // Template to add a component by constructing it with given arguments.



}
