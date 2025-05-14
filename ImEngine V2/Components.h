#pragma once

#include "ECS.h"
#include "ImEngine.h"
namespace IE {

    class TransformComponent : public Component {
    public:

        void GuiRender() override;
        void Render() override;
        void Serialize(std::ostream& out) override;
        void Deserialize(const std::string& in)override;

        void SetPosition(Vector3 position) { m_Position = position; }
        void SetRotation(Vector3 rotation) { m_Rotation = rotation; }
        void SetScale(Vector3 scale) { m_Scale = scale; }


        Matrix GetWorldTransform();

        Matrix GetLocalTransform();

        Vector3 GetWorldPosition();

        Vector3 GetWorldScale();

        Vector3 GetWorldRotationEuler();


        Vector3 GetForwardVector();

        Vector3 GetUpVector();

        Vector3 GetRightVector();

        std::string m_Name() override { return "TransformComponent"; };
        Vector3 m_Position = { 0, 0, 0 };
        Vector3 m_Rotation = { 0, 0, 0 };
        Vector3 m_Scale = { 1, 1, 1 };

        void DrawGizmos();
    };

    class RenderComponent : public Component {
    public:

        void OnAttach() override;

        void Serialize(std::ostream& out) override;
        void Deserialize(const std::string& in)override;

        void GuiRender() override;
        void Render() override;

        std::string m_Name() override { return "RenderComponent"; };
        std::shared_ptr<Model> m_Model = nullptr;

    private:
        std::string m_ModelPath;
        std::shared_ptr<Material> m_Mat = nullptr;
    };

    class CameraComponent : public Component {
    public:
        void GuiRender() override;

        void OnAttach() override;

        void Serialize(std::ostream& out) override;
        void Deserialize(const std::string& in)override;

        std::string m_Name() override { return "CameraComponent"; };
        const float& GetFOV() {
            return FOV;
        }
        void SetFOV(float s) { FOV = s; };
    private:
        float FOV = 70.f;
    };

    class CollisionComponent : public Component {
    public:
        std::string m_Name() override { return "CollisionComponent"; };
    private:
    };

    class MovementComponent : public Component {
    public:
        void Update() override;
        std::string m_Name() override { return "MovementComponent"; };
    private:
    };

    
    class ScriptComponent :public Component {
    protected:
        std::string filePath;
        std::string ComponentName;
    public:
        std::string m_Name() override { return "ScriptComponent :" + ComponentName; };

        void Update()override;
        void Render()override;
        void Start() override;
        void GuiRender() override;

        void Serialize(std::ostream& out) override;
        void Deserialize(const std::string& in)override;
    };
}
