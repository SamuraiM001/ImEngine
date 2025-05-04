#include "RenderLayers.h"
#include "SaveManager.h"
#include "rlgl.h"
#include "Profiler.h"
#include <raymath.h>
#include "Components.h"

#pragma region GameRendering

void GameLayer::OnRender() {

    if (!m_Runtime)
    BeginTextureMode(*GetFrameBuffer());



    ClearBackground(BLACK);
    BeginMode3D(*Get3DCamera());
    DrawGrid(20, 1.0f);

    Profiler::Get().Begin("Rendering Scene: "+GetScene()->GetName() );
    for (auto& [type, obj] : GetScene()->GetEntities()) { 
        obj->Render();
    }
    Profiler::Get().End("Rendering Scene: " + GetScene()->GetName());

    EndMode3D();



    if (!m_Runtime)
        EndTextureMode();


}

void GameLayer::OnEditorUpdate() {

    for (auto& [type, obj] : GetScene()->GetEntities()) {
        obj->EditorUpdate();
    }
}

void GameLayer::OnUpdate() {
    if (GetScene()->GetCurrentCamera() != nullptr) {
        auto* camera = GetScene()->GetCurrentCamera();

        m_3DCamera.position = camera->GetComponent<IE::TransformComponent>()->m_Position;
        m_3DCamera.target = Vector3Add(camera->GetComponent<IE::TransformComponent>()->m_Position, camera->GetComponent<IE::TransformComponent>()->GetForwardVector());
        m_3DCamera.up = camera->GetComponent<IE::TransformComponent>()->GetUpVector();
        m_3DCamera.fovy = 70.0f;
    }

    for (auto& [type, obj] : GetScene()->GetEntities()) {
        obj->Update();
    }
}
 void GameLayer::OnAttach() {
    IE::SaveManager::LoadSceneFromAFile(GetScene(), IE::Core::m_WorkFolder + IE::Core::m_StartScene);
}

void GameLayer::OnDetach() {
    IE::SaveManager::SaveSceneToAFile(GetScene());
}

#pragma endregion
