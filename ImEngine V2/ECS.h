#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <string>
#include <raylib.h>

namespace IE {

    class Component {
    public:
        virtual ~Component() = default;
    };

    class Object {
    public:
        Object(uint32_t id) : m_ID(id) { m_Model = LoadModelFromMesh(GenMeshCube(1,1,1)); }
        bool isSelected = false;

        void Select() { isSelected = true; };
        void UnSelect() { isSelected = false;};

        uint32_t GetID() const { return m_ID; }

        template<typename T, typename... Args>
        T* AddComponent(Args&&... args);

        template<typename T>
        T* GetComponent();


        Model* GetModel() { return &m_Model; };
        std::unordered_map<std::type_index, std::unique_ptr<Component>>& GetAllComponents();

    public:
        //setters
        void SetTransform(Matrix Transform);
        void SetPositon(Vector3 Positon);
        void SetScale(Vector3 Scale);

    public:
        std::string m_Name;
        Vector3 m_Position = { 0,0,0 };
        Vector3 m_Rotation = { 0,0,0 };
        Vector3 m_Scale = { 1,1,1 };

    private:
        uint32_t m_ID;
        std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
        Mesh m_Mesh;
        Model m_Model;
    };

}
