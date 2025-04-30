#pragma once

#include "ECS.h"
#include "ImEngine.h"
namespace IE {

    class RenderComponent : public Component {
    public:

        void GuiRender() override;
        void Render() override;
        std::string m_Name() override { return "RenderComponent"; };
    private:
        std::string m_ModelPath;
        std::shared_ptr<Model> m_Model = nullptr;
        std::shared_ptr<Material> m_Mat = nullptr;
    };

    class CameraComponent : public Component {
    public:
        void GuiRender() override;
        std::string m_Name() override { return "CameraComponent"; };
        bool isMain = false;
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
    public:
        static std::string StaticName() { }


    };
}
