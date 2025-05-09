
#include "Editor.h"
#include "ConsoleLog.h"
#include "ImEngine.h"
#include "Profiler.h"
#include "Constants.h"

void ImGuiLayer::SetupImGuiStyle(){

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO();

    ImFont* font = io.Fonts->AddFontFromMemoryTTF(
        (void*)mainfont_ttf,       
        mainfont_ttf_len,          
        18.0f                      
    ); 
    if (font)io.FontDefault = font;

    rlImGuiReloadFonts();
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

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;





    ImVec4 red = ImVec4(0.95f, 0.15f, 0.25f, 1.00f);
    ImVec4 redHover = ImVec4(0.95f, 0.15f, 0.25f, 0.70f);
    ImVec4 redLight = ImVec4(0.95f, 0.15f, 0.25f, 0.40f);
    ImVec4 bgMain = ImVec4(0.09f, 0.09f, 0.09f, 1.00f); 
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
    style.Colors[ImGuiCol_TitleBgActive] = bgDark;
    style.Colors[ImGuiCol_TitleBgCollapsed] = bgDark;

    style.Colors[ImGuiCol_MenuBarBg] = bgDark;
    style.Colors[ImGuiCol_ScrollbarBg] = bgDark;
    style.Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.05f, 0.15f, 0.30f, 1.00f);

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


    m_ResourceManager.LoadDirectory(IE::Core::m_WorkFolder);
}

void ImGuiLayer::OnDetach(){
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SaveIniSettingsToDisk(io.IniFilename);
}

void ImGuiLayer::OnEditorUpdate() {
    HandleBasicInput();
}

void ImGuiLayer::HandleBasicInput() {
    if (IsKeyPressed(KEY_DELETE)) {
        auto selected = m_Editor->GetSelectedObject();
        if (!selected.empty()) {
            auto x = selected[0]; // Save before clearing
            m_Editor->ClearSelections();
            m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene()->DestroyEntity(x->GetID());
        }
        else {
        }

    }
}

void ImGuiLayer::OnRender() {
    rlImGuiBegin();

    ClearBackground(BLACK);

    //ImGui::ShowDemoWindow();
    
    DrawMainDockspace();
    DrawMainMenuBar();
    DrawViewport();
    DrawProjectView();
    DrawProfiler();
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
                IE::SaveManager::SaveSceneToAFile(m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene());
                std::string selectedFilePath = ResourceManager::OpenFile("imscene");
                if (!selectedFilePath.empty()) {
                    m_Editor->ClearSelections();
                    IE::SaveManager::LoadSceneFromAFile(m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene(),selectedFilePath);
                }
            }
            if (ImGui::MenuItem("Save")) {
                IE::SaveManager::SaveSceneToAFile(m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene());
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    std::string name = m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene()->GetName();
    if (name == "") name = "Scene View";
    ImGui::Begin((name + "###SceneViewDock").c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("TabContextMenu");
    }

    if (ImGui::BeginPopup("TabContextMenu")) {
        DrawSceneSettings();
        ImGui::EndPopup();
    }

    // Handle mouse lock for camera control
    if (ImGui::IsWindowHovered() && IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !isMouseLocked) {
        DisableCursor();
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
        ImGui::SetWindowFocus();
        isMouseLocked = true;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && ImGui::IsWindowHovered())
        m_Editor->ClearSelections();

    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && isMouseLocked) {
        EnableCursor();
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        isMouseLocked = false;
    }

    if (isMouseLocked)
        m_Editor->HandleCameraMovementInput();

    ImVec2 availableSize = ImGui::GetContentRegionAvail();
    RenderTexture* framebuffer = m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetFrameBuffer();

    // Resize framebuffer if size changed
    if (static_cast<int>(availableSize.x) != framebuffer->texture.width ||
        static_cast<int>(availableSize.y) != framebuffer->texture.height) {
        UnloadRenderTexture(*framebuffer);
        *framebuffer = LoadRenderTexture(
            static_cast<int>(availableSize.x),
            static_cast<int>(availableSize.y)
        );
    }

    // Draw the image directly, filling the entire area
    ImGui::Image(
        (ImTextureID)(uintptr_t)&framebuffer->texture,
        availableSize,
        ImVec2(0, 1),  // bottom-left
        ImVec2(1, 0)   // top-right (flip Y)
    );

    // Overlay UI like Play/Stop buttons
    DrawViewportButtons(availableSize, availableSize);

    ImGui::End();
    ImGui::PopStyleVar();
}


void ImGuiLayer::DrawViewportButtons(const ImVec2& availableSize, const ImVec2& framebufferSize) {
    ImVec2 buttonSize = ImVec2(100, 30); 
    float margin = 10.0f; 

    ImVec2 buttonPos = ImVec2(margin, margin * 4);

    ImGui::SetCursorPos(buttonPos);

    if (ImGui::Button("Play", buttonSize)) {
        m_Editor->GetRuntimeManager()->Start(this);
        IE::SaveManager::SaveSceneToAFile(m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene());
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
            m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene()->CreateEntity();
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

    auto& objects = m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene()->GetEntities();

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8));

    for (auto& [id, obj] : objects)
    {
        if (!obj->GetParent())
            DrawObjectNode(obj.get(), 0);
    }

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


            for (const auto& [typeId, component] : obj->GetAllComponents()) {
                std::string typeName = component->m_Name();
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
                ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(50, 60, 80, 255));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, IM_COL32(30, 40, 60, 255));
                ImGui::PushStyleColor(ImGuiCol_HeaderActive, IM_COL32(0, 180, 255, 255));
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(220, 240, 255, 255));
                ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 180, 150, 80)); 
                ImGui::PushStyleColor(ImGuiCol_BorderShadow, IM_COL32(0, 0, 0, 0)); 

                if (ImGui::CollapsingHeader(typeName.c_str())) {
                    ImGui::PopStyleVar(1);
                    ImGui::PopStyleColor(6);

                    const float leftMargin = 20.0f;
                    const float barWidth = 4.0f;
                    const float barSpacing = 2.0f;


                    ImVec2 startCursor = ImGui::GetCursorScreenPos();

                    ImGui::BeginGroup();

                    ImGui::Dummy({ 1, 4 });
                    ImGui::Indent(leftMargin);
                    component->GuiRender();
                    ImGui::Dummy({ 1, 4 });

                    ImGui::EndGroup();

                    ImVec2 endCursor = ImGui::GetCursorScreenPos();

                    ImVec2 barStart = ImVec2(startCursor.x - barSpacing - barWidth, startCursor.y);
                    ImVec2 barEnd = ImVec2(barStart.x + barWidth, endCursor.y);

                    ImGui::GetWindowDrawList()->AddRectFilled(
                        barStart,
                        barEnd,
                        IM_COL32(100, 200, 255, 255),
                        2.0f
                    );
                }
                else {
                    ImGui::PopStyleVar(1);
                    ImGui::PopStyleColor(6);

                }
            }
            ImGui::Dummy({ 5,5 });
            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("AddComponentPopup");
            }

            // 'obj' is a pointer to an IE::Object
            if (ImGui::BeginPopup("AddComponentPopup")) {
                const auto& allComponents = IE::ComponentRegistry::Get().GetAll();

                for (const auto& [name, getType] : allComponents) {
                    std::type_index type = getType();

                    if (!name.empty() && ImGui::MenuItem(name.c_str())) {
                        std::unique_ptr<IE::Component> comp = IE::ComponentRegistry::Get().CreateComponent(name);
                        if (comp) {
                            comp->SetOwner(obj);
                            comp->OnAttach();
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
    ImGui::Separator();

    if (ImGui::Button("< Back")) {
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

        float iconOffsetX = (cellSize - thumbnailSize) * 0.5f;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconOffsetX);

        // Button Style
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(60, 60, 60, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(100, 100, 100, 255));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(120, 120, 120, 255));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

        if (ImGui::Button(icon, ImVec2(thumbnailSize, thumbnailSize))) {
            if (entry.type == ResourceManager::Dir)
                m_ResourceManager.LoadDirectory(entry.fullPath);
            else
                IE_LOG("Clicked file: {}", entry.fullPath);
        }

        if (entry.type == ResourceManager::File && ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("ASSET_FILE", entry.fullPath.c_str(), entry.fullPath.size() + 1);
            ImGui::Text("Dragging: %s", entry.name.c_str());
            ImGui::EndDragDropSource();
        }

        ImGui::PopFont();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        std::string name = entry.name;
        float maxWidth = thumbnailSize + 10.0f;
        float nameWidth = ImGui::CalcTextSize(name.c_str()).x;

        float textScale = 1.f;

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconOffsetX);
        ImGui::SetWindowFontScale(textScale);
        ImGui::TextWrapped(name.c_str());
        ImGui::SetWindowFontScale(1.0f);

        ImGui::EndGroup();

        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::Columns(1);
    ImGui::End();
}


void ImGuiLayer::DrawProfiler() {
    ImGui::Begin("Profiler");

    const auto& timings = Profiler::Get().GetTimings();

    if (ImGui::BeginTable("ProfilerTable", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Section");
        ImGui::TableSetupColumn("Call Count");
        ImGui::TableSetupColumn("Avg Time (ms)");
        ImGui::TableHeadersRow();

        for (const auto& [name, info] : timings) {
            double avgTime = (info.totalTime / 1000.0) /  info.callCount; // microseconds to ms

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0); ImGui::TextUnformatted(name.c_str());
            ImGui::TableSetColumnIndex(1); ImGui::Text("%d", info.callCount);
            ImGui::TableSetColumnIndex(2); ImGui::Text("%.3f", avgTime);
        }

        ImGui::EndTable();
    }

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


void ImGuiLayer::DrawSceneSettings(){
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_Once); // Set initial size
    ImGui::Begin("Scene Settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize); // Allow resize

    static char rename_buffer[128] = "";
    ImGui::Text("Scene Name:");
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputText("##SceneName", rename_buffer, IM_ARRAYSIZE(rename_buffer),
        ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
        m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene()->SetName(rename_buffer);
    }

    ImGui::Separator();

    // Camera Component List
    IE::Scene* scene = m_Editor->GetRenderStack()->GetLayer<GameLayer>()->GetScene();
    ImGui::Text("Camera Objects:");
    ImGui::BeginChild("CameraList", ImVec2(0, 150), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

    for (auto& [id, object] : scene->GetEntities()) {
        if (object->GetComponent<IE::CameraComponent>() != nullptr) {
            bool isCurrent = (scene->GetCurrentCamera() && scene->GetCurrentCamera()->GetID() == id);
            std::string label = object->m_Name + " (ID: " + std::to_string(object->GetID()) + ") " + (isCurrent ? " (StartUp)" : "");

            if (ImGui::Selectable(label.c_str(), isCurrent)) {
                scene->SetCurrentCamera(object.get());
            }
        }
    }

    ImGui::EndChild();

    // Escape key closes the popup
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
        ImGui::CloseCurrentPopup();

    ImGui::End(); // Scene Settings
}



void ImGuiLayer::DrawObjectNode(IE::Object* object, int depth)
{
    if (!object) return;

    // Optional safeguard against excessive depth
    if (depth > 1000)
    {
        ImGui::Text("Max depth exceeded!");
        return;
    }

    ImGui::PushID(object->GetID());

    ImGui::BeginGroup();
    ImGui::Indent(depth * 20.0f);

    bool isSelected = object->isSelected;
    if (isSelected)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.9f, 0.6f));

    if (ImGui::Selectable(object->m_Name.c_str(), isSelected, 0, ImVec2(0, 24)))
    {
        m_Editor->ClearSelections();
        m_Editor->Select(object);
    }

    if (isSelected)
        ImGui::PopStyleColor();

    // Begin drag source
    if (ImGui::BeginDragDropSource())
    {
        IE::Object* ptr = object;
        ImGui::SetDragDropPayload("OBJECT_HIERARCHY", &ptr, sizeof(IE::Object*));
        ImGui::Text("Dragging %s", object->m_Name.c_str());
        ImGui::EndDragDropSource();
    }

    // Accept drop
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OBJECT_HIERARCHY"))
        {
            IE::Object* dropped = *(IE::Object**)payload->Data;

            if (dropped && dropped != object && dropped->GetParent() != object)
            {
                dropped->SetParent(object); 
            }
        }

        ImGui::EndDragDropTarget();
    }

    ImGui::Unindent(depth * 20.0f);
    ImGui::EndGroup();

    // Recursively draw children
    for (auto& child : object->GetChildren())
    {
        if (child) // null check
            DrawObjectNode(child, depth + 1);
    }

    ImGui::PopID();
}

#pragma endregion