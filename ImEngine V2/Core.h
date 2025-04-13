#pragma once  

#include "Base.h"

#include "raylib.h"  

#include "imgui/imgui.h"  
#include "imgui/rlImGui.h"  

#include <iostream>  
#include <string>

namespace IE{
    enum CameraMode {
        THREE_D,
        TWO_D
    };
    class Core{
    public:
            static std::string m_WorkFolder;
            void Initialize(int argc, char* argv[]);
            void Shutdown();
    };
}