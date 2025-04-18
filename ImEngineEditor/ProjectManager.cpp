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
                std::string iniSourcePath = "\\imgui.ini"; 
                std::string iniDestPath = fullPath + "\\imgui.ini";

                IE_LOG(iniSourcePath);


                // Check if the imgui.ini file exists before copying
                if (std::filesystem::exists(iniSourcePath)) {
                    try {
                        std::filesystem::copy(iniSourcePath, iniDestPath);
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error copying imgui.ini: " << e.what() << std::endl;
                    }
                }
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
