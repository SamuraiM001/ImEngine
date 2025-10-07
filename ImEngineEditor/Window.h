#pragma once

#include "imgui/imgui.h"
#include "imgui/rlImGui.h"

#include "raylib.h"

class ImGuiLayer;

class Window {
public:
    Window(ImGuiLayer* lay) { m_Layer = lay; };
    virtual void OnDraw() {};
    ImGuiLayer* m_Layer;
    void Activate() {
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

    w_SceneSettings(ImGuiLayer* lay) : Window(lay) {};

	void OnDraw() override;
};

class w_Viewport : public Window{
public:
    w_Viewport(ImGuiLayer* lay) : Window(lay), SceneSettings(lay) {};
	bool isMouseLocked = false;
	void OnDraw() override;
	w_SceneSettings SceneSettings;
	void DrawViewportButtons(const ImVec2& availableSize, const ImVec2& framebufferSize);
};

class w_Hierarchy :public Window {
public:
    w_Hierarchy(ImGuiLayer* lay) : Window(lay) {};
    void OnDraw() override;
    void DrawObjectNode(IE::Object* object, int depth);
};

class w_Plugins :public Window {
public:
    w_Plugins(ImGuiLayer* lay) :Window(lay) {};
    void OnDraw() override;


};
