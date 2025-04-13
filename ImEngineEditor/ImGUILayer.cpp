#include "Editor.h"
#include "ImEngine.h"
#include "ConsoleLog.h"

void ImGuiLayer::OnAttach() {
    rlImGuiSetup(true);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);
    style.FramePadding = ImVec2(6, 4);
    style.FrameRounding = 2.0f;
    style.GrabRounding = 1.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
}

void ImGuiLayer::OnUpdate() {
    HandleBasicInput();
}

void ImGuiLayer::HandleBasicInput() {
}

void ImGuiLayer::OnRender() {
    rlImGuiBegin();
    ClearBackground(BLACK);

    DrawMainDockspace();
    DrawMainMenuBar();
    DrawViewport();
    DrawProjectView();
    DrawHierarchy();
    DrawLog();
    DrawProperities();



    rlImGuiEnd();
}

#pragma region ImGui Windows

void ImGuiLayer::DrawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        // --- File Menu ---
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New")) {
            }
            if (ImGui::MenuItem("Open")) {
            }
            if (ImGui::MenuItem("Save")) {
            }
            if (ImGui::MenuItem("Exit")) {
                CloseWindow();  
            }
            ImGui::EndMenu(); 
        }

        // --- Edit Menu ---
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
            }
            if (ImGui::MenuItem("Redo")) {
            }
            ImGui::EndMenu();
        }

        // --- Tools Menu ---
        if (ImGui::BeginMenu("Tools")) {
            if (ImGui::MenuItem("Settings")) {
            }
            if (ImGui::MenuItem("Preferences")) {
            }
            ImGui::EndMenu();
        }

    }
    ImGui::EndMainMenuBar();
}

void ImGuiLayer::DrawMainDockspace() {
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGuiWindowFlags dockspace_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::Begin("DockSpaceHost", nullptr, dockspace_flags);

    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
    ImGui::PopStyleVar(2);


}

void ImGuiLayer::DrawViewport() {
    ImGui::Begin("Viewport");

    // Handle mouse lock for camera control
    if (ImGui::IsWindowHovered() && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !isMouseLocked) {
        DisableCursor();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; 
        isMouseLocked = true;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered())m_Editor->ClearSelections();

    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && isMouseLocked) {
        EnableCursor();
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;  
        isMouseLocked = false;
    }

    // Handle camera movement input when mouse is locked
    if (isMouseLocked) m_Editor->HandleCameraMovementInput();  

    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    RenderTexture* framebuffer = m_Editor->GetFrameBuffer();

    constexpr float aspectRatio = 16.0f / 9.0f;
    ImVec2 imageSize = availableSize;

    // Force 16:9 ratio
    float availableAspect = availableSize.x / availableSize.y;
    if (availableAspect > aspectRatio) {
        imageSize.x = availableSize.y * aspectRatio;
        imageSize.y = availableSize.y;
    }
    else {
        imageSize.x = availableSize.x;
        imageSize.y = availableSize.x / aspectRatio;
    }

    // Only recreate framebuffer if the size has actually changed
    int newWidth = static_cast<int>(imageSize.x);
    int newHeight = static_cast<int>(imageSize.y);

    if (newWidth != framebuffer->texture.width || newHeight != framebuffer->texture.height) {
        UnloadRenderTexture(*framebuffer);
        *framebuffer = LoadRenderTexture(newWidth, newHeight);
    }

    // Center the image in the viewport
    ImVec2 cursorPos = ImGui::GetCursorPos();
    ImVec2 offset = {
        (availableSize.x - imageSize.x) / 2.0f,
        (availableSize.y - imageSize.y) / 2.0f
    };
    ImGui::SetCursorPos(ImVec2(cursorPos.x + offset.x, cursorPos.y + offset.y));

    // Draw the image
    ImGui::Image(
        (ImTextureID)(uintptr_t)&framebuffer->texture,
        imageSize,
        ImVec2(0, 1), ImVec2(1, 0)  // Flip vertically
    );

    // Draw the buttons
    DrawViewportButtons(availableSize, imageSize);

    ImGui::End();
}

void ImGuiLayer::DrawViewportButtons(const ImVec2& availableSize, const ImVec2& framebufferSize) {
    ImVec2 buttonSize = ImVec2(100, 30); 
    float margin = 10.0f; 

    ImVec2 buttonPos = ImVec2(margin, margin * 4);

    ImGui::SetCursorPos(buttonPos);

    if (ImGui::Button("Play", buttonSize)) {
        m_Editor->GetRuntimeManager()->Start(this);
        std::cout << "Play clicked!" << std::endl;
    }

    buttonPos.x += buttonSize.x + margin; 
    ImGui::SetCursorPos(buttonPos);  

    if (ImGui::Button("Stop", buttonSize)) {
        m_Editor->GetRuntimeManager()->Stop();
        std::cout << "Stop clicked!" << std::endl;
    }

    buttonPos.x += buttonSize.x + margin;
    ImGui::SetCursorPos(buttonPos);  

    if (ImGui::BeginCombo("Create Entity", "Select Entity")) {
        if (ImGui::Selectable("Empty Entity")) {
            m_Editor->GetScene()->CreateEntity();
        }
        ImGui::EndCombo();
    }
}


void ImGuiLayer::DrawHierarchy()
{
    ImGui::Begin("Hierarchy");

    // Top margin
    ImGui::Dummy(ImVec2(0.0f, 10.0f)); // Adds vertical spacing at the top

    // Clear selection if clicking empty space
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        m_Editor->ClearSelections();

    auto& entities = m_Editor->GetScene()->GetEntities();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8)); // Make text vertically centered
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Use larger font (make sure font[0] is large enough)

    for (auto& [id, entity] : entities)
    {
        ImGui::PushID(id);

        bool isSelected = entity->isSelected;

        if (isSelected)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.9f, 0.6f));

        // Start horizontal layout for icon + name
        ImGui::BeginGroup();

        ImGui::BeginChild("Icon", ImVec2(20, 20), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::Dummy(ImVec2(16, 16)); // Placeholder for icon
        ImGui::EndChild();

        ImGui::SameLine();

        if (ImGui::Selectable(entity->m_Name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, 24)))
        {
            if (ImGui::IsMouseDoubleClicked(0)) {
                // Optional: Expand
            }
            else {
                m_Editor->Select(entity.get());
            }
        }

        ImGui::EndGroup();

        if (isSelected)
            ImGui::PopStyleColor();

        ImGui::PopID();
    }

    ImGui::PopFont();
    ImGui::PopStyleVar();

    ImGui::End();
}


void ImGuiLayer::DrawProperities() {
    ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);
    ImGui::Begin("Properties");

    if (!m_Editor->GetSelectedObject().empty()) {
        IE::Object* obj = m_Editor->GetSelectedObject()[0];
        if (obj) {
            // Editable Name
            char nameBuffer[128] = {};
            strncpy_s(nameBuffer, sizeof(nameBuffer), obj->m_Name.c_str(), _TRUNCATE);

            if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
                obj->m_Name = nameBuffer;
            }

            ImGui::Separator();
            ImGui::Text("Transform");

            // Position / Rotation / Scale
            ImGui::DragFloat3("Position", &obj->m_Position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &obj->m_Rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &obj->m_Scale.x, 0.1f);

            ImGui::Separator();
            ImGui::Text("Components");

            // Loop through components and show placeholder UI
            for (const auto& [typeId, component] : obj->GetAllComponents()) {
                std::string typeName = typeId.name();

                if (ImGui::TreeNode(typeName.c_str())) {
                    ImGui::Text("Component UI coming soon...");
                    ImGui::TreePop();
                }
            }

            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponentPopup");
            }

            if (ImGui::BeginPopup("AddComponentPopup")) {
                if (ImGui::MenuItem("Empty Component")) {
                    // Add a basic component here
                    // obj->AddComponent<YourComponent>();
                }
                ImGui::EndPopup();
            }
        }
    }

    ImGui::End();
}


void ImGuiLayer::DrawProjectView(){

}

void ImGuiLayer::DrawLog()
{
    ImGui::Begin("Developer Console");

    static char inputBuffer[256] = "";
    const std::string& log = IE::Log::Get().GetBuffer();

    // Console log display
    ImGui::BeginChild("LogScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImGui::TextUnformatted(log.c_str());
    ImGui::SetScrollHereY(1.0f); // Always scroll to bottom
    ImGui::EndChild();

    ImGui::PushItemWidth(-1);
    ImGui::SetNextItemWidth(-1);

    if (ImGui::InputText("##ConsoleInput", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        std::string command = inputBuffer;
        if (!command.empty())
        {
            IE::Log::Get().ExecuteCommand(command);
        }
        ImGui::SetKeyboardFocusHere(-1); // Keep focus on the input box after enter
        inputBuffer[0] = '\0'; // Clear input

    }
    ImGui::PopItemWidth();

    ImGui::End();
}




#pragma endregion


