#include "ProjectManager.h"
#include "ResourceManager.h"
#include "Editor.h"
#include "Runtime.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include "raylib.h"
void SetupImguiStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // Base colors
    colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 0.98f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

    style.WindowRounding = 5.3f;
    style.FrameRounding = 2.3f;
    style.ScrollbarRounding = 5.0f;
    style.GrabRounding = 2.0f;
    style.TabRounding = 3.0f;
}

std::vector<std::string> projectPaths;
std::string projectsFile = "projects.list";
bool hasEnding(std::string const& fullString, std::string const& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}
void ProjectManager::LoadProjects() {
    projectPaths.clear();

    std::ifstream file(projectsFile);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && std::filesystem::exists(line) && hasEnding(line,".improject")) {
            projectPaths.push_back(line);
        }
    }
}

std::string ProjectManager::GetProjectPath() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(800, 600, "Select a Project");

    rlImGuiSetup(true);
    LoadProjects();

    std::string selectedPath;
    static char projectName[128] = "NewProject";
    SetupImguiStyle();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        rlImGuiBegin();

        ImGui::SetNextWindowPos({ 0, 0 });
        ImGui::SetNextWindowSize({ (float)GetScreenWidth(), (float)GetScreenHeight() });
        ImGui::Begin("Project Picker", nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

        ImGui::Text("Select an Existing Project:");
        ImGui::Separator();

        int columns = (GetScreenWidth() > 800) ? 2 : 1;
        ImGui::Columns(columns, nullptr, false);
        for (int i = 0; i < projectPaths.size(); ++i) {
            ImGui::PushID(i);
            if (ImGui::Button(projectPaths[i].c_str(), ImVec2(-1, 40))) {
                selectedPath = projectPaths[i];
                ImGui::PopID();
                break;
            }
            ImGui::PopID();
            ImGui::NextColumn();
        }
        ImGui::Columns(1);

        ImGui::SetCursorPosY(GetScreenHeight() - 140);
        ImGui::Separator();
        ImGui::Text("Create a New Project:");

        ImGui::PushItemWidth(200);
        ImGui::InputText("##projectname", projectName, sizeof(projectName));
        ImGui::SameLine();
        if (ImGui::Button("Create Project", ImVec2(180, 0))) {
            std::string basePath = ResourceManager::OpenFolder();
            if (!basePath.empty()) {
                std::string fullPath = basePath + "\\" + projectName;
                std::filesystem::create_directory(fullPath);

                std::string improjPath = fullPath + "\\Project.improject";
                std::ofstream improjFile(improjPath);
                improjFile << "ProjectName=" << projectName << "\n";
                improjFile << "StartupScene=\\Main.imscene\n";

                std::ofstream sceneFile(fullPath + "\\Main.imscene");
                sceneFile << "Scene \"Sapalaqius\"\n";
                sceneFile << "Entity\n";
                sceneFile << "  Name \"DefaultEntity\"\n";
                sceneFile << "  Position 0.000 0.000 0.000\n";
                sceneFile << "  Rotation 0.000 0.000 0.000\n";
                sceneFile << "  Scale 1.000 1.000 1.000\n";
                sceneFile << "  Components RenderComponent\n";

                std::ofstream out(projectsFile, std::ios::app);
                out << improjPath << "\n";

                std::string iniDestPath = fullPath + "\\imgui.ini";
                std::ofstream inipath(iniDestPath);
                inipath << "[Window][Debug##Default]\nPos=60,60\nSize=400,400\nCollapsed=0\n\n[Window][Sal]\nPos=743,565\nSize=764,159\nCollapsed=0\n\n[Window][SAKL]\nPos=682,373\nSize=664,347\nCollapsed=0\n\n[Window][Dear ImGui Demo]\nPos=1096,405\nSize=1234,661\nCollapsed=0\n\n[Window][Dear ImGui Metrics/Debugger]\nPos=1108,129\nSize=653,623\nCollapsed=0\n\n[Window][DockSpace]\nSize=1920,1020\nCollapsed=0\n\n[Window][DockSpaceRoot]\nSize=1920,1020\nCollapsed=0\n\n[Window][Toolbar]\nPos=127,42\nSize=600,50\nCollapsed=0\n\n[Window][DockspaceBackground]\nSize=1920,1020\nCollapsed=0\n\n[Window][DockspaceWindow]\nSize=1920,1020\nCollapsed=0\n\n[Window][DockSpaceHost]\nPos=0,0\nSize=1920,991\nCollapsed=0\n\n[Window][Decorated Window]\nPos=883,141\nSize=289,42\nCollapsed=0\n\n[Window][Sample Window]\nSize=1824,991\nCollapsed=0\nDockId=0x00000001,0\n\n[Window][Console]\nPos=235,622\nSize=1412,369\nCollapsed=0\nDockId=0x00000007,1\n\n[Window][Inspector]\nPos=1649,25\nSize=271,966\nCollapsed=0\nDockId=0x00000006,0\n\n[Window][Hierarchy]\nPos=8,38\nSize=495,945\nCollapsed=0\nDockId=0x00000007,0\n\n[Window][Viewport]\nPos=359,25\nSize=1188,480\nCollapsed=0\nDockId=0x0000000D,0\n\n[Window][Hierarchu]\nPos=0,25\nSize=330,966\nCollapsed=0\nDockId=0x00000009,0\n\n[Window][Properities]\nPos=0,25\nSize=420,966\nCollapsed=0\nDockId=0x00000007,0\n\n[Window][Properties]\nPos=1558,38\nSize=354,945\nCollapsed=0\nDockId=0x00000004,0\n\n[Window][Debug Log]\nPos=539,672\nSize=1008,319\nCollapsed=0\nDockId=0x0000000C,0\n\n[Window][Developer Console]\nPos=505,669\nSize=1051,314\nCollapsed=0\nDockId=0x00000010,0\n\n[Window][Sapalaqius]\nPos=505,38\nSize=1051,629\nCollapsed=0\nDockId=0x0000000F,0\n\n[Window][Project]\nPos=505,669\nSize=1051,314\nCollapsed=0\nDockId=0x00000010,1\n\n[Window][Profiler]\nPos=505,669\nSize=1051,314\nCollapsed=0\nDockId=0x00000010,2\n\n[Docking][Data]\nDockSpace ID=0x94EF87A8 Window=0x34F970D7 Pos=8,38 Size=1904,945 Split=X\n  DockNode ID=0x00000003 Parent=0x94EF87A8 SizeRef=1564,966 Split=X\n    DockNode ID=0x00000009 Parent=0x00000003 SizeRef=330,966 Selected=0x9F9F1B7F\n    DockNode ID=0x0000000A Parent=0x00000003 SizeRef=1588,966 Split=X\n      DockNode ID=0x00000005 Parent=0x0000000A SizeRef=1647,966 Split=X\n        DockNode ID=0x00000001 Parent=0x00000005 SizeRef=1824,991 Selected=0x987D410F\n        DockNode ID=0x00000002 Parent=0x00000005 SizeRef=94,991 Split=X Selected=0x8C72BEA8\n          DockNode ID=0x00000007 Parent=0x00000002 SizeRef=495,966 CentralNode=1 Selected=0xBABDAE5E\n          DockNode ID=0x00000008 Parent=0x00000002 SizeRef=1051,966 Split=Y Selected=0xC450F867\n            DockNode ID=0x0000000B Parent=0x00000008 SizeRef=393,645 Split=Y Selected=0xCCA4E27C\n              DockNode ID=0x0000000D Parent=0x0000000B SizeRef=1188,480 Selected=0xC450F867\n              DockNode ID=0x0000000E Parent=0x0000000B SizeRef=1188,484 Split=Y Selected=0x9C21DE82\n                DockNode ID=0x0000000F Parent=0x0000000E SizeRef=1055,629 Selected=0xA0E0BDFD\n                DockNode ID=0x00000010 Parent=0x0000000E SizeRef=1055,314 Selected=0xCCA4E27C\n            DockNode ID=0x0000000C Parent=0x00000008 SizeRef=393,319 Selected=0x2D9FB795\n      DockNode ID=0x00000006 Parent=0x0000000A SizeRef=271,966 Selected=0x36DC96AB\n  DockNode ID=0x00000004 Parent=0x94EF87A8 SizeRef=354,966 Selected=0x8C72BEA8";

                selectedPath = improjPath;
                break;
            }
        }

        ImGui::End();
        rlImGuiEnd();
        EndDrawing();

        if (!selectedPath.empty()) break;
    }

    rlImGuiShutdown();
    CloseWindow();

    return selectedPath;
}
