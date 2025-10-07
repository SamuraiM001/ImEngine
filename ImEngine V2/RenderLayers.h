#pragma once
#include "Core.h"
#include "Render.h"
#include "Scene.h"
#include "ECS.h"


class GameLayer :public IE::RenderLayer {
protected:
    IE::Scene m_Scene;
    RenderTexture framebuffer;
    Camera3D  m_3DCamera = {
        { 4.0f, 4.0f, 4.0f },
       { 0.0f, 0.0f, 0.0f },
       { 0.0f, 1.0f, 0.0f },
        45.0f,
       CAMERA_PERSPECTIVE
    };
    Camera2D  m_2DCamera = { 0 };
    IE::CameraMode m_CamM = IE::CameraMode::THREE_D;
    IE::Object* m_SelectedObject;
public:
    bool m_Runtime = false;
    RenderTexture* GetFrameBuffer() { return &framebuffer; };
    IE::Object* GetSelectedObject() { return m_SelectedObject; }
    IE::Object* SetSelectedObject(IE::Object* obj) { m_SelectedObject = obj; return m_SelectedObject; }
    IE::CameraMode GetCameraMode() { return m_CamM; };
    Camera2D* Get2DCamera() { return &m_2DCamera; }
    Camera3D* Get3DCamera() { return &m_3DCamera; }
    IE::Scene* GetScene() { return &m_Scene; }

    GameLayer() {}
    void OnRender()override;
    void OnEditorUpdate()override;
    void OnUpdate()override;
    void OnAttach()override;
    void OnDetach()override;
    void OnStart();
};


