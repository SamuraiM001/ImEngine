#pragma once

#include "ImEngine.h"
#include "Runtime.h"
#include "ResourceManager.h"


#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

#include "raylib.h"

class Editor;

class GameLayer :public IE::RenderLayer {
    Editor* m_Editor;  
public:
    GameLayer(Editor* editor) : m_Editor(editor) {
        IE_ASSERT(editor != nullptr, "Editor pointer cannot be null!");
    }
    void OnRender()override;
    void OnUpdate()override;
};


class ImGuiLayer : public IE::RenderLayer {
public:
    // Use a raw pointer (Editor owns this layer, so no ownership needed)
    ImGuiLayer(Editor* editor) : m_Editor(editor) {
        IE_ASSERT(editor != nullptr, "Editor pointer cannot be null!");
    }
    bool isMouseLocked = false;
    void DrawMainMenuBar();
    void DrawMainDockspace();
    void DrawViewport();
    void DrawHierarchy();
    void DrawProperities();
    void DrawProjectView();
    void DrawLog();
    void DrawViewportButtons(const ImVec2& availableSize, const ImVec2& framebufferSize);

public:
    void HandleBasicInput();
public:
    void OnRender() override;
    void OnAttach() override;
    void OnUpdate() override;
private:
    ResourceManager m_ResourceManager;
    Editor* m_Editor; 

};


class Editor : public IE::App
{
private:
    RenderTexture framebuffer;
    IE::Core m_Core;
    RuntimeManager m_RuntimeManager;

    Camera3D  m_3DCamera = {
         { 4.0f, 4.0f, 4.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
         45.0f,
        CAMERA_PERSPECTIVE
    };
    Camera2D  m_2DCamera = { 0 };
    IE::CameraMode m_CamM = IE::CameraMode::THREE_D;
    
    IE::Scene m_Scene;
    std::vector <IE::Object*> m_selectedObjects;
public:
	Editor() {};
	void Initialize(int argc, char* argv[]) override;
	void Run() override;
	void Shutdown()override;

    void HandleCameraMovementInput();
    RenderTexture* GetFrameBuffer() { return &framebuffer; };
    
    void Select(IE::Object* obj) {obj->Select() ; m_selectedObjects.push_back(obj); };
    void ClearSelections() { for (auto& x : m_selectedObjects)x->UnSelect(); m_selectedObjects.clear(); }
    RuntimeManager* GetRuntimeManager() { return &m_RuntimeManager; };
public:
    //Getters
    IE::CameraMode GetCameraMode() { return m_CamM; };
    Camera2D* Get2DCamera() { return &m_2DCamera; }
    Camera3D* Get3DCamera() { return &m_3DCamera; }
    IE::Scene* GetScene() { return &m_Scene;}
    std::vector <IE::Object*> GetSelectedObject() {return m_selectedObjects;};
};