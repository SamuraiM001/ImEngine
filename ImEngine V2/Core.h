#pragma once  

#include "Base.h"

#include "raylib.h"  

#include "imgui/imgui.h"  
#include "imgui/imgui_impl_raylib.h"
#include "imgui/rlImGui.h"  


#include <string>

namespace IE{
    enum CameraMode {
        THREE_D,
        TWO_D
    };
    class Core{
    private:
        void ParseProjectFile(std::string folder);
    public:
            static std::string m_ProjectName;
            static std::string m_WorkFolder;
            static std::string m_ProjectFile;
            static std::string m_StartScene;
            void Initialize(int argc, char* argv[]);
            void Shutdown();
    };
}