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

    class Collision : public Component {
    public:
        void Render() override;
        std::string m_Name() override { return "TestComp"; };
    private:
    };

    class ScriptComponent :public Component {
    protected:
        std::string filePath;
    public:
        static std::string StaticName() { }


    };
}
