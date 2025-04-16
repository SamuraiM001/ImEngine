﻿
#include "Editor.h"
#include "ConsoleLog.h"
#include "ImEngine.h"



void SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    // Modern spacing & rounding
    style.WindowMinSize = ImVec2(180, 30);
    style.FramePadding = ImVec2(10, 6);
    style.ItemSpacing = ImVec2(10, 8);
    style.ItemInnerSpacing = ImVec2(6, 5);
    style.Alpha = 1.0f;
    style.WindowRounding = 12.0f;
    style.FrameRounding = 10.0f;
    style.IndentSpacing = 12.0f;
    style.ColumnsMinSpacing = 10.0f;
    style.GrabMinSize = 16.0f;
    style.GrabRounding = 10.0f;
    style.ScrollbarSize = 16.0f;
    style.ScrollbarRounding = 12.0f;
    style.TabRounding = 8.0f;

    
    ImVec4 red = ImVec4(0.95f, 0.15f, 0.25f, 1.00f);
    ImVec4 redHover = ImVec4(0.95f, 0.15f, 0.25f, 0.70f);
    ImVec4 redLight = ImVec4(0.95f, 0.15f, 0.25f, 0.40f);
    ImVec4 bgMain = ImVec4(0.09f, 0.09f, 0.09f, 1.00f); // #121212
    ImVec4 bgDark = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    ImVec4 bgLight = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    ImVec4 borderDim = ImVec4(0.20f, 0.20f, 0.20f, 0.5f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 0.95f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4f, 0.4f, 0.4f, 1.00f);

    style.Colors[ImGuiCol_WindowBg] = bgMain;
    style.Colors[ImGuiCol_ChildBg] = bgMain;
    style.Colors[ImGuiCol_PopupBg] = bgLight;

    style.Colors[ImGuiCol_Border] = borderDim;
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    style.Colors[ImGuiCol_FrameBg] = bgLight;
    style.Colors[ImGuiCol_FrameBgHovered] = redHover;
    style.Colors[ImGuiCol_FrameBgActive] = red;

    style.Colors[ImGuiCol_TitleBg] = bgDark;
    style.Colors[ImGuiCol_TitleBgActive] = red;
    style.Colors[ImGuiCol_TitleBgCollapsed] = bgDark;

    style.Colors[ImGuiCol_MenuBarBg] = bgDark;
    style.Colors[ImGuiCol_ScrollbarBg] = bgDark;
    style.Colors[ImGuiCol_ScrollbarGrab] = redLight;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = redHover;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = red;

    style.Colors[ImGuiCol_CheckMark] = red;
    style.Colors[ImGuiCol_SliderGrab] = redLight;
    style.Colors[ImGuiCol_SliderGrabActive] = red;

    style.Colors[ImGuiCol_Button] = redLight;
    style.Colors[ImGuiCol_ButtonHovered] = redHover;
    style.Colors[ImGuiCol_ButtonActive] = red;

    style.Colors[ImGuiCol_Header] = redLight;
    style.Colors[ImGuiCol_HeaderHovered] = redHover;
    style.Colors[ImGuiCol_HeaderActive] = red;

    style.Colors[ImGuiCol_Separator] = borderDim;
    style.Colors[ImGuiCol_SeparatorHovered] = redHover;
    style.Colors[ImGuiCol_SeparatorActive] = red;

    style.Colors[ImGuiCol_ResizeGrip] = redLight;
    style.Colors[ImGuiCol_ResizeGripHovered] = redHover;
    style.Colors[ImGuiCol_ResizeGripActive] = red;

    style.Colors[ImGuiCol_Tab] = redLight;
    style.Colors[ImGuiCol_TabHovered] = redHover;
    style.Colors[ImGuiCol_TabActive] = red;
    style.Colors[ImGuiCol_TabUnfocused] = bgDark;
    style.Colors[ImGuiCol_TabUnfocusedActive] = redLight;

    style.Colors[ImGuiCol_PlotLines] = redLight;
    style.Colors[ImGuiCol_PlotLinesHovered] = red;
    style.Colors[ImGuiCol_PlotHistogram] = redLight;
    style.Colors[ImGuiCol_PlotHistogramHovered] = red;

    style.Colors[ImGuiCol_TextSelectedBg] = redLight;


}

void ImGuiLayer::OnAttach() {
    rlImGuiSetup(true);
    SetupImGuiStyle();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(0, 0);
    style.FramePadding = ImVec2(6, 4);
    style.FrameRounding = 2.0f;
    style.GrabRounding = 1.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

    m_ResourceManager.LoadDirectory(IE::Core::m_WorkFolder);
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
                IE::SaveManager::SaveSceneToAFile(m_Editor->GetScene());
                std::string selectedFilePath = ResourceManager::OpenDirectory();
                if (!selectedFilePath.empty()) {
                    m_Editor->ClearSelections();
                    IE::SaveManager::LoadSceneFromAFile(m_Editor->GetScene(),selectedFilePath);
                }
            }
            if (ImGui::MenuItem("Save")) {
                IE::SaveManager::SaveSceneToAFile(m_Editor->GetScene());
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
    std::string name = m_Editor->GetScene()->GetName()!="" ? m_Editor->GetScene()->GetName() : "Viewport";
    ImGui::Begin(name.c_str());

    // Handle mouse lock for camera control
    if (ImGui::IsWindowHovered() && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !isMouseLocked) {
        DisableCursor();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse; 
        ImGui::SetWindowFocus();
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
        IE_LOG("Runtime Started");
    }

    buttonPos.x += buttonSize.x + margin; 
    ImGui::SetCursorPos(buttonPos);  

    if (ImGui::Button("Stop", buttonSize)) {
        m_Editor->GetRuntimeManager()->Stop();
        IE_LOG("Runtime Stopped From Engine");
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

    ImGui::Dummy(ImVec2(0.0f, 10.0f)); 

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
        m_Editor->ClearSelections();

    auto& entities = m_Editor->GetScene()->GetEntities();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8)); 
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); 

    for (auto& [id, entity] : entities)
    {
        ImGui::PushID(id);

        bool isSelected = entity->isSelected;

        if (isSelected)
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.9f, 0.6f));

        ImGui::BeginGroup();

        // Add left margin here
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); // Adjust this value to control margin

        // Start horizontal layout
        ImGui::Text("%s", ICON_FA_GAMEPAD); // Icon as label
        ImGui::SameLine();

        // Entity name selectable
        if (ImGui::Selectable(entity->m_Name.c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(0, 24)))
        {
            if (ImGui::IsMouseDoubleClicked(0)) {
                // Handle double click
            }
            else {
                m_Editor->ClearSelections();
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
    ImGui::Dummy({ 20,20 });
    if (!m_Editor->GetSelectedObject().empty()) {
        IE::Object* obj = m_Editor->GetSelectedObject()[0];
        if (obj) {
            char nameBuffer[128] = {};
            strncpy_s(nameBuffer, sizeof(nameBuffer), obj->m_Name.c_str(), _TRUNCATE);

            if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
                obj->m_Name = nameBuffer;
            }

            ImGui::Separator();
            ImGui::Text("Transform");

            ImGui::DragFloat3("Position",    &obj->m_Position.x, 0.1f);
            ImGui::DragFloat3("Rotation", &obj->m_Rotation.x, 0.1f);
            ImGui::DragFloat3("Scale", &obj->m_Scale.x, 0.1f);

            ImGui::Separator();
            ImGui::Text("Components");

            // Loop through components and show placeholder UI
            for (const auto& [typeId, component] : obj->GetAllComponents()) {
                std::string typeName = typeId.name();

                if (ImGui::TreeNode(typeName.c_str())) {
                    component->GuiRender();
                    ImGui::TreePop();

                }
            }

            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponentPopup");
            }

            // 'obj' is a pointer to an IE::Object
            if (ImGui::BeginPopup("AddComponentPopup")) {
                const auto& allComponents = IE::ComponentRegistry::Get().GetAll();

                for (const auto& [name, getType] : allComponents) {
                    std::type_index type = getType();

                    if (ImGui::MenuItem(name.c_str())) {
                        std::unique_ptr<IE::Component> comp = IE::ComponentRegistry::Get().CreateComponent(name);
                        if (comp) {
                            comp->SetOwner(obj);
                            obj->GetAllComponents()[type] = std::move(comp);
                        }
                    }

                }
                ImGui::EndPopup();
            }




        }
    }

    ImGui::End();
}


void ImGuiLayer::DrawProjectView() {
    ImGui::Begin("Project");
    ImGui::Dummy({5,5});
    ImGui::Separator();
    // 🔙 Back Button and Path Bar
    if (ImGui::Button(ICON_FA_ARROW_LEFT " Back")) {
        m_ResourceManager.GoBack();
    }
    ImGui::SameLine();
    ImGui::TextUnformatted(m_ResourceManager.GetCurrentPath().c_str());
    ImGui::Separator();
    ImGui::Dummy({ 5,5 });

    float padding = 20.0f;
    float thumbnailSize = 96.0f;
    float cellSize = thumbnailSize + padding;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = (int)(panelWidth / cellSize);
    if (columnCount < 1) columnCount = 1;

    ImGui::Columns(columnCount, 0, false);

    const auto& files = m_ResourceManager.GetDirectory();
    for (const auto& entry : files) {
        ImGui::PushID(entry.fullPath.c_str());

        const char* icon = (entry.type == ResourceManager::Dir) ? ICON_FA_FOLDER : ICON_FA_FILE;

        ImGui::BeginGroup();

        // Center button
        float iconOffsetX = (cellSize - thumbnailSize) * 0.5f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconOffsetX);

        // 🧱 Custom Button Look
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60, 60, 60, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(100, 100, 100, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(120, 120, 120, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);

        if (ImGui::Button(icon, ImVec2(thumbnailSize, thumbnailSize))) {
            if (entry.type == ResourceManager::Dir)
                m_ResourceManager.LoadDirectory(entry.fullPath);
            else
                IE_LOG("Clicked file: {}", entry.fullPath);
        }

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        // 📏 Scale text if it's too long
        std::string name = entry.name;
        float maxWidth = thumbnailSize + 10.0f;
        float nameWidth = ImGui::CalcTextSize(name.c_str()).x;

        float textScale = nameWidth > maxWidth ? maxWidth / nameWidth : 1.0f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconOffsetX);
        ImGui::SetWindowFontScale(textScale); // Shrink only the label
        ImGui::TextWrapped(name.c_str());
        ImGui::SetWindowFontScale(1.0f); // Reset

        ImGui::EndGroup();

        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::Columns(1);
    ImGui::End();
}


void ImGuiLayer::DrawLog()
{
    ImGui::Begin("Developer Console");

    static char inputBuffer[256] = "";
    const std::string& log = IE::Log::Get().GetBuffer();

    ImGui::BeginChild("LogScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

    std::istringstream stream(log);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find("[ERROR]") != std::string::npos)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 80, 80, 255)); // Red
        else if (line.find("[WARN]") != std::string::npos)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 200, 0, 255)); // Yellow
        else if (line.find("[SUCCESS]") != std::string::npos)
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 100, 255)); // Green

        ImGui::TextUnformatted(line.c_str());

        if (line.find("[ERROR]") != std::string::npos ||
            line.find("[WARN]") != std::string::npos ||
            line.find("[SUCCESS]") != std::string::npos)
            ImGui::PopStyleColor();
    }

    ImGui::SetScrollHereY(1.0f);
    ImGui::EndChild();

    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("##ConsoleInput", inputBuffer, IM_ARRAYSIZE(inputBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        std::string command = inputBuffer;
        if (!command.empty()) {
            IE::Log::Get().ExecuteCommand(command);
            ImGui::SetKeyboardFocusHere(-1);
        }
        inputBuffer[0] = '\0';
    }
    ImGui::PopItemWidth();

    ImGui::End();
}


#pragma endregion


