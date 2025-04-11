#pragma once  

#include "Base.h"

#include "raylib.h"  

#include "imgui/imgui.h"  
#include "imgui/rlImGui.h"  

#include <iostream>  


namespace IE{
    enum CameraMode {
        THREE_D,
        TWO_D
    };
    class Core{
    public:
            void Initialize();
            void Shutdown();

    };
}