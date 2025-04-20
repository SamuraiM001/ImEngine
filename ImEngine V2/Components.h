#pragma once

#include "ECS.h"
#include "ImEngine.h"
namespace IE {

    class RenderComponent : public Component {
    public:
        static std::string StaticName() { return "RenderComponent"; }

        void GuiRender() override;
        void Render() override;

    private:
        std::string m_ModelPath;
        std::shared_ptr<Model> m_Model = nullptr;
    };

    class Collision : public Component {
    public:
        static std::string StaticName() { return "RenderComponent"; }
        void Render() override;
    };

    class ScriptComponent :public Component {
    protected:
        std::string filePath;
    public:
        static std::string StaticName() { }


    };
}
