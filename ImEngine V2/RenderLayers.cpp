#include "RenderLayers.h"
#include "SaveManager.h"
#include "rlgl.h"
#include "Profiler.h"

#pragma region GameRendering

void GameLayer::OnRender() {

    if (!m_Runtime)
    BeginTextureMode(*GetFrameBuffer());



    ClearBackground(BLACK);
    BeginMode3D(*Get3DCamera());
    DrawGrid(20, 1.0f);

    Profiler::Get().Begin("Scene Render Objects");
    for (auto& [type, obj] : GetScene()->GetEntities()) {
        rlEnableDepthTest();
        DrawBillboard(m_3DCamera, *obj->GetBillboardTexture(), obj->m_Position,  .5f,RED );
        
        rlDisableDepthTest();
        
        obj->Render();
    }
    Profiler::Get().End("Scene Render Objects");

    EndMode3D();



    if (!m_Runtime)
        EndTextureMode();


}

void GameLayer::OnEditorUpdate() {

    for (auto& [type, obj] : GetScene()->GetEntities()) {
        obj->EditorUpdate();
    }
}

void GameLayer::OnUpdate(){
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
