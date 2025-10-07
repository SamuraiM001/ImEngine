#pragma once

#include "ImEngine.h"
#include "Runtime.h"
#include "ResourceManager.h"

#include "Window.h"



class Editor;


class ImGuiLayer : public IE::RenderLayer {
public:

    ImGuiLayer(Editor* editor) : m_Editor(editor) {
        IE_ASSERT(editor != nullptr, "Editor pointer cannot be null!");
    }
    std::vector<std::unique_ptr<Window>> m_Windows;
    static void SetupImGuiStyle();
    void DrawMainMenuBar();
    void DrawMainDockspace();

    void DrawPlugins();
    void DrawProperities();
    void DrawProjectView();
    void DrawProfiler();
    void DrawShaderController();
    void DrawLog();

public:

    void OpenFileEntry(ResourceManager::ResourceEntry rE);

public:
    void HandleBasicInput();
public:
    void OnRender() override;
    void OnAttach() override;
    void OnDetach() override;
    void OnEditorUpdate() override;
    std::unordered_map<std::string, std::function<void(std::string)>> m_FunctionsByExtensions;


    Editor* m_Editor; 
protected:    

    ResourceManager m_ResourceManager;


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
    
    void Select(IE::Object* obj) { GetRenderStack()->GetLayer<GameLayer>()->SetSelectedObject(obj); m_selectedObjects.push_back(obj); };
    void ClearSelections() { GetRenderStack()->GetLayer<GameLayer>()->SetSelectedObject(nullptr); m_selectedObjects.clear(); }
    RuntimeManager* GetRuntimeManager() { return &m_RuntimeManager; };
public:
    //Getters
    std::vector <IE::Object*> GetSelectedObject() {return m_selectedObjects;};
};