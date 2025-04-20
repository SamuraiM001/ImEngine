#include "RenderLayers.h"
#include "SaveManager.h"

#pragma region GameRendering

void GameLayer::OnRender() {

    if (!m_Runtime)
    BeginTextureMode(*GetFrameBuffer());



    ClearBackground(BLACK);
    BeginMode3D(*Get3DCamera());
    DrawGrid(20, 1.0f);


    for (auto& [type, obj] : GetScene()->GetEntities()) {
        obj->Render();
    }

    EndMode3D();

    if (!m_Runtime)
        EndTextureMode();


}

void GameLayer::OnUpdate() {

    for (auto& [type, obj] : GetScene()->GetEntities()) {
        obj->EditorUpdate();
    }
}

void GameLayer::OnAttach() {
    IE::SaveManager::LoadSceneFromAFile(GetScene(), IE::Core::m_WorkFolder + IE::Core::m_StartScene);
}

void GameLayer::OnDetach() {
    IE::SaveManager::SaveSceneToAFile(GetScene());

}

#pragma endregion
