#define RUNTIME_EXPORTS
#include "Runtime.h"
#include "Editor.h"
#include "ResourceManager.h"
#include "SaveManager.h"
#include <raylib.h>
#include <atomic>


static std::atomic<bool> s_RequestingExit = false;
static std::atomic<bool> s_WindowInitialized = false;
static GameLayer m_GameLayer;
static IE::Core m_Core;

RUNTIME_API bool IsRuntimeRequestingExit() {
    return s_RequestingExit.load();
}


RUNTIME_API void InitRuntime(std::string ProjectPath) {
    if (s_WindowInitialized.load()) {
        IE_LOG("Runtime already initialized, skipping InitWindow.");
        s_RequestingExit = false;
        return;
    }
    IE::ComponentRegistry::Get().RegisterComponents();

    IE_LOG(ProjectPath);
    const char* fakeArgv[] = { "ImEngineEditor.exe",ProjectPath.c_str()};

    m_Core.Initialize(2, const_cast<char**>(fakeArgv));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 600, "Runtime View");

    IE::SaveManager::LoadSceneFromAFile(m_GameLayer.GetScene(),IE::Core::m_WorkFolder + IE::Core::m_StartScene);
    m_GameLayer.m_Runtime = true;

    s_WindowInitialized = true; 
    s_RequestingExit = false;
}


RUNTIME_API void TickRuntime(float deltaTime) {
    if (!s_WindowInitialized.load()) return;

    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        s_RequestingExit = true;
    }

    UpdateCamera(m_GameLayer.Get3DCamera(), CAMERA_THIRD_PERSON);

    BeginDrawing();

    m_GameLayer.OnUpdate();
    m_GameLayer.OnRender();



    EndDrawing();

    if (s_RequestingExit.load()) {
        Shutdown();
    }
}

RUNTIME_API void Shutdown() {
    if (s_WindowInitialized.exchange(false)) {
        CloseWindow();
    }
    s_RequestingExit = true;

}

