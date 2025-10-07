#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>
#include "Base.h"
#include <raylib.h> 

namespace IE {

    class Object;
    class Scene;

    class Component {
    private:
        Object* m_Owner = nullptr;
        uint32_t m_ID;
    public:
        virtual std::string m_Name() { return "SA"; };
        virtual ~Component() = default;

        virtual void Start() {}
        virtual void Update() {}
        virtual void EditorUpdate() {}
        virtual void Render() {}
        virtual void RenderSelection() {};
        virtual void GuiRender() {};

        virtual void Serialize(std::ostream& out) {}
        virtual void Deserialize(const std::string& in) {};

        virtual void OnAttach() {};
        virtual void OnDetach() {};
       
        void SetOwner(Object* owner) { m_Owner = owner; }
        Object* GetOwner() const { return m_Owner; }


        bool m_isActive = true;
    };

    class Object {
    public:
        Object(uint32_t id) : m_ID(id) , m_BillboardText(LoadTexture("textures/Logo.png")) {}

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args) {
            static_assert(std::is_base_of<Component, T>::value,
                "Template type T must derive from Component");

            std::unique_ptr<T> comp = std::make_unique<T>(std::forward<Args>(args)...);

            comp->SetOwner(this);
            comp->OnAttach();
            T* rawPtr = comp.get();
            m_Components[typeid(T)] = std::move(comp);
            return rawPtr;
        }

        template<typename T>
        T* GetComponent() {
            auto it = m_Components.find(typeid(T));
            if (it != m_Components.end()) {
                return dynamic_cast<T*>(it->second.get());
            }
            return nullptr;
        }

        std::unordered_map<std::type_index, std::unique_ptr<Component>>& GetAllComponents() {return m_Components;}



        void Update();
        void EditorUpdate();
        void Start();
        
        void AddChild(Object* child);

        void SetParent(Object* newParent);
        Object* GetParent() { return m_Parent; }

        void Render();
        void RenderSelection();

        Texture2D* GetBillboardTexture() { return &m_BillboardText; };
   
        bool IsAncestorOf(IE::Object* potentialDescendant);




        uint32_t GetID() const { return m_ID; }
        std::vector<Object*>& GetChildren() { return m_Children; };

    public:

        void SetScene(Scene* scene) { m_Scene = scene; }
        Scene* GetScene() const { return m_Scene; }

    private:
        Scene* m_Scene = nullptr;

    public:
        std::string m_Name;
        uint32_t m_ID;
    private:
        std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;

        Texture2D m_BillboardText;
        std::vector<Object*> m_Children;
        Object* m_Parent = nullptr;
    };



}
