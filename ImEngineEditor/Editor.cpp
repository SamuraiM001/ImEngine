#include "Editor.h"
#include "ImEngine.h"
#include "ConsoleLog.h"
#include "ResourceManager.h"
#include "Profiler.h"
#include "Constants.h"

#pragma region GameRendering

void GameLayer::OnRender() {


    Profiler::Get().Begin("Game Layer Render");
    RenderTexture* framebuffer = m_Editor->GetRenderStack()->GetLayer<ImGuiLayer>()->GetFrameBuffer();
    if (!framebuffer) return;

    BeginTextureMode(*framebuffer);  

    ClearBackground(BLACK);
    BeginMode3D(*Get3DCamera());
    DrawGrid(20, 1.0f);


    for (auto& [type, obj] : m_Editor->GetScene()->GetEntities()) {
        obj->Render();
    }
    
    EndMode3D();

    EndTextureMode();  
    Profiler::Get().End("Game Layer Render");

}

void GameLayer::OnUpdate() {
    Profiler::Get().Begin("Game Layer Update");

    for (auto& [type, obj] : m_Editor->GetScene()->GetEntities()) {
        obj->EditorUpdate();
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
    IE::SaveManager::SaveSceneToAFile(GetScene());
    CloseWindow();
}

void Editor::HandleCameraMovementInput()
{

    if (GetRenderStack()->GetLayer<GameLayer>()->GetCameraMode() == IE::CameraMode::THREE_D) {

        UpdateCameraPro(GetRenderStack()->GetLayer<GameLayer>()->Get3DCamera(),
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
    if (GetRenderStack()->GetLayer<GameLayer>()->GetCameraMode() == IE::CameraMode::TWO_D)
    {
        // Move the 2D camera using arrow keys or WASD
        float moveSpeed = 200.0f * GetFrameTime();  // Speed of movement, scaled by frame time for smooth movement

        // Move the camera with WASD keys
        if (IsKeyDown(KEY_W)) GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->target.y -= moveSpeed;
        if (IsKeyDown(KEY_S)) GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->target.y += moveSpeed;
        if (IsKeyDown(KEY_A)) GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->target.y -= moveSpeed;
        if (IsKeyDown(KEY_D)) GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->target.y += moveSpeed;

        // Zoom in and out with the mouse wheel
        float zoomSpeed = 0.05f;  // Zoom speed
        GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->zoom += GetMouseWheelMove() * zoomSpeed;
        if (GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->zoom < 0.1f) GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->zoom = 0.1f;  // Prevent zooming out too much
        if (GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->zoom > 5.0f) GetRenderStack()->GetLayer<GameLayer>()->Get2DCamera()->zoom = 5.0f;   // Prevent zooming in too much
    }
}

