#define RUNTIME_EXPORTS
#include "Runtime.h"
#include <raylib.h>
#include <atomic>

// Thread-safe state management
static std::atomic<bool> s_RequestingExit = false;
static std::atomic<bool> s_WindowInitialized = false;
static Camera3D s_Camera;

RUNTIME_API bool IsRuntimeRequestingExit() {
    return s_RequestingExit.load();
}


RUNTIME_API void InitRuntime(void* windowHandle) {
    if (s_WindowInitialized.load()) {
        s_RequestingExit = false;
        return;
    }

    // Initialize new window
    SetTraceLogLevel(LOG_WARNING);  // Reduce log spam
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(600, 600, "Runtime View");

    s_Camera.position = { 10.0f, 10.0f, 10.0f };
    s_Camera.target = { 0.0f, 0.0f, 0.0f };
    s_Camera.up = { 0.0f, 1.0f, 0.0f };
    s_Camera.fovy = 45.0f;
    s_Camera.projection = CAMERA_PERSPECTIVE;

    s_WindowInitialized = true;
    s_RequestingExit = false;
}

RUNTIME_API void TickRuntime(float deltaTime) {
     
    if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) {
        s_RequestingExit = true;
    }

    UpdateCamera(&s_Camera, CAMERA_ORBITAL);

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(s_Camera);
    DrawGrid(10, 1.0f);
    EndMode3D();
    EndDrawing();
}

RUNTIME_API void Shutdown() {
    if (s_WindowInitialized.exchange(false)) {
        CloseWindow();
    }
    s_RequestingExit = true;
}

