#pragma once

#include "ImEngine.h"
#include "Runtime.h"
#include "ResourceManager.h"


#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

#include "raylib.h"


class Editor;


class ImGuiLayer : public IE::RenderLayer {
public:

    ImGuiLayer(Editor* editor) : m_Editor(editor) {
        IE_ASSERT(editor != nullptr, "Editor pointer cannot be null!");
    }
    static void SetupImGuiStyle();
    bool isMouseLocked = false;
    void DrawMainMenuBar();
    void DrawMainDockspace();
    void DrawViewport();
    void DrawHierarchy();
    void DrawProperities();
    void DrawProjectView();
    void DrawProfiler();
    void DrawLog();
    void DrawObjectNode(IE::Object* object, int depth);
    void DrawViewportButtons(const ImVec2& availableSize, const ImVec2& framebufferSize);

public:
    void HandleBasicInput();
public:
    void OnRender() override;
    void OnAttach() override;
    void OnDetach() override;
    void OnEditorUpdate() override;
private:    
    ResourceManager m_ResourceManager;
    Editor* m_Editor; 


};


class Editor : public IE::App{
private:
    IE::Core m_Core;
    RuntimeManager m_RuntimeManager;
    
    std::vector <IE::Object*> m_selectedObjects;
public:
	Editor() {};
	void Initialize(int argc, char* argv[]) override;
	void Run() override;
	void Shutdown()override;

    void HandleCameraMovementInput();
    
    void Select(IE::Object* obj) {obj->Select() ; m_selectedObjects.push_back(obj); };
    void ClearSelections() { for (auto& x : m_selectedObjects)x->UnSelect(); m_selectedObjects.clear(); }
    RuntimeManager* GetRuntimeManager() { return &m_RuntimeManager; };
public:
    //Getters
    std::vector <IE::Object*> GetSelectedObject() {return m_selectedObjects;};
};