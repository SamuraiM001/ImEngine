#pragma once

#include "ECS.h"
#include "ImEngine.h"

namespace IE {

    class RenderComponent : public Component {
    public:
        static std::string StaticName() { return "RenderComponent"; }
        void GuiRender()override;
        void Render() override;
    };
    class Collision : public Component {
    public:
        static std::string StaticName() { return "RenderComponent"; }
        void Render() override;
    };
}
