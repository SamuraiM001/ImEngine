#include "Editor.h"
#include "ImEngine.h"
#include "ConsoleLog.h"
#include "ResourceManager.h"
#include "Profiler.h"
#include "Constants.h"

#pragma region GameRendering

void GameLayer::OnRender() {


    Profiler::Get().Begin("Game Layer Render");
    RenderTexture* framebuffer = m_Editor->GetFrameBuffer();
    if (!framebuffer) return;

    BeginTextureMode(*framebuffer);  

    ClearBackground(BLACK);

    if (m_Editor->GetCameraMode() == IE::CameraMode::THREE_D)
        BeginMode3D(*m_Editor->Get3DCamera());
    if (m_Editor->GetCameraMode() == IE::CameraMode::TWO_D)
        BeginMode2D(*m_Editor->Get2DCamera());

    DrawGrid(20, 1.0f);
    for (auto& [type, obj] : m_Editor->GetScene()->GetEntities()) {
        obj->Render();
    }
    if (m_Editor->GetCameraMode() == IE::CameraMode::THREE_D)
        EndMode3D();
    if (m_Editor->GetCameraMode() == IE::CameraMode::TWO_D)
        EndMode2D();

    EndTextureMode();  
    Profiler::Get().End("Game Layer Render");

}

void GameLayer::OnUpdate() {
    Profiler::Get().Begin("Game Layer Update");

    for (auto& [type, obj] : m_Editor->GetScene()->GetEntities()) {
        obj->Update();
    }
    Profiler::Get().End("Game Layer Update");
}

#pragma endregion

void Editor::Initialize(int argc, char* argv[]) {
    //Hooking the logger to cout and cerr
    IE::Log::Get().Hook();
    
    //Registering CurrentComponents
    IE::ComponentRegistry::RegisterComponents();
    
    //Initializing Core
    m_Core.Initialize(argc, argv);
    //Initializing Window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);  
    InitWindow(800, 600, "Im Engine");
    MaximizeWindow();
    
    //Loading the icon
    Image icon = LoadImage("textures/Logo.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    //Loading Main Scene
    IE::SaveManager::LoadSceneFromAFile(GetScene(),IE::Core::m_WorkFolder + IE::Core::m_StartScene);

    //Pushing RenderLayers
    PushLayer(std::make_unique<GameLayer>(this));
    PushOverlay(std::make_unique<ImGuiLayer>(this));
}

void Editor::Run() {
    while (!WindowShouldClose()) {
        Profiler::Get().BeginFrame();
        BeginDrawing();

        m_rStack.Update();
        m_rStack.Render();

        EndDrawing();
    }
}

void Editor::Shutdown() {
    m_RuntimeManager.Unload();
    IE::Log::Get().Unhook();
    m_Core.Shutdown();
    CloseWindow();
}

void Editor::HandleCameraMovementInput()
{

    if (m_CamM == IE::CameraMode::THREE_D){

        UpdateCameraPro(&m_3DCamera,
            Vector3({
                 ((IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * 0.1f - (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * 0.1f),
                 ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * 0.1f - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * 0.1f),
                 ((IsKeyDown(KEY_E) || IsKeyDown(KEY_SPACE)) * 0.1f - (IsKeyDown(KEY_Q) || IsKeyDown(KEY_LEFT_CONTROL)) * 0.1f),
                }),
                Vector3({
                    GetMouseDelta().x * 0.05f,
                    GetMouseDelta().y * 0.05f,
                    0.0f
                    }),
            GetMouseWheelMove() * -2.0f
        );
        // Camera movement sensitivity
        float moveSpeed = 10.0f * GetFrameTime();

        // Optional: You can add custom movement or behavior, such as restricting the camera's position
        // for example, you could restrict the Z-axis range or other behaviors.
    }

    // 2D Camera Movement (manual controls)
    if (m_CamM == IE::CameraMode::TWO_D)
    {
        // Move the 2D camera using arrow keys or WASD
        float moveSpeed = 200.0f * GetFrameTime();  // Speed of movement, scaled by frame time for smooth movement

        // Move the camera with WASD keys
        if (IsKeyDown(KEY_W)) m_2DCamera.target.y -= moveSpeed;
        if (IsKeyDown(KEY_S)) m_2DCamera.target.y += moveSpeed;
        if (IsKeyDown(KEY_A)) m_2DCamera.target.x -= moveSpeed;
        if (IsKeyDown(KEY_D)) m_2DCamera.target.x += moveSpeed;

        // Zoom in and out with the mouse wheel
        float zoomSpeed = 0.05f;  // Zoom speed
        m_2DCamera.zoom += GetMouseWheelMove() * zoomSpeed;
        if (m_2DCamera.zoom < 0.1f) m_2DCamera.zoom = 0.1f;  // Prevent zooming out too much
        if (m_2DCamera.zoom > 5.0f) m_2DCamera.zoom = 5.0f;   // Prevent zooming in too much
    }
}

