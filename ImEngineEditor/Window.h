#pragma once

#include "imgui/imgui.h"
#include "imgui/rlImGui.h"
#include "raylib.h"

#include "ConnectionManager.h"


class ImGuiLayer;

class Window {
public:
    Window(ImGuiLayer* lay, bool StartActivity) : m_Layer(lay), isActive(StartActivity) {};
    virtual void OnDraw() {};
    ImGuiLayer* m_Layer;
    virtual void Activate() {
        isActive = true;
    };
    void Deactivate() {
        isActive = false;
    };
    bool GetActivity() { return isActive; }
protected:
    bool isActive = true;

};

class w_SceneSettings : public Window {
public:

    w_SceneSettings(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};

	void OnDraw() override;
};

class w_Viewport : public Window{
public:
    w_Viewport(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) , SceneSettings(lay,StartActivity) {};
	bool isMouseLocked = false;
	void OnDraw() override;
	w_SceneSettings SceneSettings;
	void DrawViewportButtons(const ImVec2& availableSize, const ImVec2& framebufferSize);
};

class w_Hierarchy :public Window {
public:
    w_Hierarchy(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    void OnDraw() override;
    void DrawObjectNode(IE::Object* object, int depth);
};

class w_Plugins :public Window {
public:
    w_Plugins(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    std::vector<PluginInfo> m_Plugins;
    void Activate() override;
    std::atomic<bool> isLoading = false;

    void OnDraw();



};

class w_Properties :public Window {
public:
    w_Properties(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    void OnDraw() override;
};

class w_ProjectView :public Window {
public:
    w_ProjectView(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    void OnDraw() override;
};

class w_Log :public Window {
public:
    w_Log(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    void OnDraw() override;
};

class w_Profiler :public Window {
public:
    w_Profiler(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    void OnDraw() override;
};

class w_ShaderController :public Window {
public:
    w_ShaderController(ImGuiLayer* lay, bool StartActivity) : Window(lay, StartActivity) {};
    void OnDraw() override;
};

