#include "Components.h"
#include <filesystem>
#include <sstream>

using namespace IE;


#pragma region Transform C
Vector3 TransformComponent::GetForwardVector() {
    Matrix rotationMatrix = MatrixRotateXYZ(m_Rotation);
    return {
        rotationMatrix.m8,
        rotationMatrix.m9,
        rotationMatrix.m10
    };
}

Vector3 TransformComponent::GetUpVector() {
    Matrix rotationMatrix = MatrixRotateXYZ(m_Rotation);
    return {
        rotationMatrix.m4,
        rotationMatrix.m5,
        rotationMatrix.m6
    };
}

Vector3 TransformComponent::GetRightVector() {
    Matrix rotationMatrix = MatrixRotateXYZ(m_Rotation);
    return {
        rotationMatrix.m0,
        rotationMatrix.m1,
        rotationMatrix.m2
    };
}

void TransformComponent::GuiRender()
{
    float controlWidth = 90.0f;

    ImGui::Text("Position");
    ImGui::PushID("Position");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("X", &m_Position.x, 0.1f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("Y", &m_Position.y, 0.1f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("Z", &m_Position.z, 0.1f);
    ImGui::PopStyleColor();

    ImGui::PopID();



    ImGui::Text("Rotation");
    ImGui::PushID("Rotation");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("X", &m_Rotation.x, 0.1f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("Y", &m_Rotation.y, 0.1f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("Z", &m_Rotation.z, 0.1f);
    ImGui::PopStyleColor();

    ImGui::PopID();

    ImGui::Text("Scale");
    ImGui::PushID("Scale");

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("X", &m_Scale.x, 0.1f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("Y", &m_Scale.y, 0.1f);
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 255, 255));
    ImGui::SetNextItemWidth(controlWidth);
    ImGui::DragFloat("Z", &m_Scale.z, 0.1f);
    ImGui::PopStyleColor();



    ImGui::PopID();



}

void TransformComponent::RenderSelection() {
    DrawGizmos();
}

void TransformComponent::DrawGizmos() {
    if (!GetOwner()) return;


    Matrix world = GetWorldTransform();


    // Draw gizmo axes
    Vector3 origin = Vector3Transform(Vector3Zero(), world);
    Vector3 right = origin + GetRightVector();
    Vector3 up = origin + GetUpVector();
    Vector3 forward = origin + GetForwardVector();

    if (isLocalMode) {
        DrawLine3D(origin, right, RED);
        DrawLine3D(origin, up, GREEN);
        DrawLine3D(origin, forward, BLUE);
    }
    else {
        DrawLine3D(origin, origin + Vector3({ 0,0,1 }), BLUE);
        DrawLine3D(origin, origin + Vector3({ 0,1,0 }), GREEN);
        DrawLine3D(origin, origin + Vector3({ 1,0,0 }), RED);
    }
}

void TransformComponent::Serialize(std::ostream& out)  {
    out << "      Position " << m_Position.x << " " << m_Position.y << " " << m_Position.z << "\n";
    out << "      Rotation " << m_Rotation.x << " " << m_Rotation.y << " " << m_Rotation.z << "\n";
    out << "      Scale " << m_Scale.x << " " << m_Scale.y << " " << m_Scale.z << "\n";
}


void TransformComponent::Deserialize(const std::string& line) {
    std::istringstream ss(line);
    std::string prefix;
    ss >> prefix;

    if (prefix == "Position") {
        ss >> m_Position.x >> m_Position.y >> m_Position.z;
    }
    else if (prefix == "Rotation") {
        ss >> m_Rotation.x >> m_Rotation.y >> m_Rotation.z;
    }
    else if (prefix == "Scale") {
        ss >> m_Scale.x >> m_Scale.y >> m_Scale.z;
    }
}


Matrix TransformComponent::GetWorldTransform()
{
    if (GetOwner() != nullptr && GetOwner()->GetParent() && GetOwner()->GetParent()->GetComponent<TransformComponent>() != nullptr)
        return GetOwner()->GetParent()->GetComponent<TransformComponent>()->GetWorldTransform() * GetLocalTransform(); // Local then parent
    else
        return GetLocalTransform();
}

Matrix TransformComponent::GetLocalTransform()
{
    // Create a scale matrix
    Matrix scaleMatrix = MatrixScale(m_Scale.x, m_Scale.y, m_Scale.z);

    // Create rotation matrices from Euler angles
    Matrix rotX = MatrixRotateX(m_Rotation.x);
    Matrix rotY = MatrixRotateY(m_Rotation.y);
    Matrix rotZ = MatrixRotateZ(m_Rotation.z);
    Matrix rotationMatrix = MatrixMultiply(MatrixMultiply(rotZ, rotY), rotX);

    // Create a translation matrix
    Matrix translationMatrix = MatrixTranslate(m_Position.x, m_Position.y, m_Position.z);

    // Final local transform: Scale → Rotate → Translate
    Matrix localTransform = MatrixMultiply(MatrixMultiply(scaleMatrix, rotationMatrix), translationMatrix);
    return localTransform;
}


Vector3 TransformComponent::GetWorldPosition() {
    return Vector3Transform(Vector3Zero(), GetWorldTransform());
}

Vector3 TransformComponent::GetWorldScale() {
    return m_Scale; // Could be expanded for parent scale in hierarchy
}

Vector3 TransformComponent::GetWorldRotationEuler() {
    return m_Rotation; // Assumes no parent influence
}


#pragma endregion

#pragma region Render C
void RenderComponent::OnAttach(){
    if (GetOwner()->GetComponent<TransformComponent>() == nullptr) {
        GetOwner()->AddComponent<TransformComponent>();
    }
    ReaplyMaterials();

}

void RenderComponent::Serialize(std::ostream& out){
    out << "      ModelPath " << m_ModelPath << '\n';
}

void RenderComponent::Deserialize(const std::string& in) {
    std::istringstream ss(in);
    std::string prefix;
    ss >> prefix;

    if (prefix == "ModelPath") {
        std::string pathPart;
        m_ModelPath.clear();
        while (ss >> pathPart) {
            if (!m_ModelPath.empty()) m_ModelPath += " ";
            m_ModelPath += pathPart;
        }

        Model rawModel = LoadModel(m_ModelPath.c_str());
        if (rawModel.meshCount == 0 && !m_ModelPath.empty()) {
            IE_LOG_ERROR("Failed to load model: "<< m_ModelPath);
        }
        m_Model = std::make_shared<Model>(rawModel);
    }
}

void RenderComponent::ReaplyMaterials(){
    IE::MaterialComponent* matComp = GetOwner()->GetComponent<MaterialComponent>();

    if (matComp) {
        std::vector<MaterialComponent::MaterialEntry>* materials = matComp->GetMaterials();
        if (materials) {
            for (MaterialComponent::MaterialEntry& m : *materials) {
                m_Model->materials[0].shader = LoadShader(m.vsPath.c_str(), m.fsPath.c_str());
            }
        }


    }
}

void RenderComponent::GuiRender()
{
    #pragma region Mesh Render

    ImGui::Text("Mesh");
    ImGui::PushID("MeshDrop");

    bool clicked = ImGui::Button(m_Model ? m_ModelPath.c_str() : "Drop .obj file here", ImVec2(-1, 32));

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("MeshContextMenu");
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
        {
            if (payload->DataSize > 0)
            {
                const char* droppedPath = static_cast<const char*>(payload->Data);
                std::string extension = std::filesystem::path(droppedPath).extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

                if(extension == ".obj"){
                    m_ModelPath = droppedPath;
                    IE_LOG(m_ModelPath);
                    Model rawModel = LoadModel(m_ModelPath.c_str());
                    if (rawModel.meshCount == 0) {
                        IE_LOG_ERROR("Failed to load model:" << m_ModelPath);
                    }
                    m_Model = std::make_shared<Model>(rawModel);
                }
                else
                {
                    ImGui::OpenPopup("InvalidFile");
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginPopup("MeshContextMenu"))
    {
        if (ImGui::MenuItem("Clear"))
        {
            m_Model.reset();
            m_ModelPath.clear();
        }

        if (m_Model)
        {
            if (ImGui::MenuItem("Reload Model"))
            {
                Model rawModel = LoadModel(m_ModelPath.c_str());
                if (rawModel.meshCount == 0) {
                    IE_LOG_ERROR("Failed to reload model: {}" << m_ModelPath);
                }
                m_Model = std::make_shared<Model>(rawModel);
            }

            if (ImGui::MenuItem("Copy Path"))
            {
                ImGui::SetClipboardText(m_ModelPath.c_str());
            }

            if (std::filesystem::exists(m_ModelPath))
            {
                if (ImGui::MenuItem("Open in Explorer"))
                {
                    std::string command = "explorer /select,\"" + m_ModelPath + "\"";
                    system(command.c_str());
                }
            }
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("InvalidFile"))
    {
        ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "Only .obj files are supported.");
        ImGui::EndPopup();
    }

    ImGui::PopID();

    #pragma endregion

}

void RenderComponent::Render() { 
    if (!m_Model || m_Model->meshCount == 0 || m_Model->meshes[0].vertexCount == 0) {
        m_Model = std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(1, 1, 1)));
    }

    
    if (GetOwner()->GetComponent<TransformComponent>()) {
        m_Model->transform = GetOwner()->GetComponent<TransformComponent>()->GetWorldTransform();
        DrawModel(*m_Model, Vector3Zero(), 1.0f, WHITE);
    }
    else {
        IE_LOG_ERROR("Render Component On Object:" +  GetOwner()->m_Name + "(ID: " + std::to_string(GetOwner()->GetID()) + ") cannot be rendered because there is no transform component" );
    }


}


#pragma endregion

#pragma region Movement C

void MovementComponent::Update()
{
    if(GetOwner()->GetComponent<TransformComponent>())
    GetOwner()->GetComponent<TransformComponent>()->m_Position = Vector3Add(GetOwner()->GetComponent<TransformComponent>()->m_Position, Vector3({ 0.1f * GetFrameTime(),0.1f * GetFrameTime(),0.1f * GetFrameTime()}));

}
#pragma endregion

#pragma region Camera C
void CameraComponent::GuiRender() {
    ImGui::PushID(this); // Ensure unique ID for this component

    bool isActiveCamera = (GetOwner()->GetScene()->GetCurrentCamera() == GetOwner());
    ImVec4 buttonColor = isActiveCamera
        ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f)  // Green if active
        : ImVec4(0.4f, 0.4f, 0.4f, 1.0f); // Gray if inactive

    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
    if (ImGui::Button(isActiveCamera ? "[ACTIVE] Set as Main" : "Set as Main Camera")) {
        GetOwner()->GetScene()->SetCurrentCamera(GetOwner());
    }
    ImGui::PopStyleColor();

    //ImGui::SameLine();
    //bool isEnabled = IsActive(); 
    //if (ImGui::Checkbox("##CameraEnabled", &isEnabled)) {
    //    SetActive(isEnabled); 
    //}
    //ImGui::SameLine();
    //ImGui::Text("Enabled");

        // Adjustable FOV (Field of View)
        float fov = GetFOV(); 
        if (ImGui::SliderFloat("Field of View", &fov, 30.0f, 120.0f, "%.1f deg")) {
            SetFOV(fov); 
        }

        /*float nearClip = GetNearClip();
        float farClip = GetFarClip();
        if (ImGui::DragFloatRange2("Clipping Planes", &nearClip, &farClip, 0.1f, 0.01f, 1000.0f, "Near: %.2f", "Far: %.2f")) {
            SetNearClip(nearClip);
            SetFarClip(farClip);
        }*/

       /* bool isOrtho = IsOrthographic();
        if (ImGui::Checkbox("Orthographic", &isOrtho)) {
            SetOrthographic(isOrtho);
        }*/

    ImGui::PopID(); 
}

void CameraComponent::OnAttach() {
    if (GetOwner()->GetComponent<TransformComponent>() == nullptr) {
        GetOwner()->AddComponent<TransformComponent>();
    }
}

void CameraComponent::Serialize(std::ostream& out){

    out << "      IsMainCam " << (GetOwner()->GetScene()->GetCurrentCamera() == GetOwner()) << '\n';
}

void CameraComponent::Deserialize(const std::string& in)
{
    std::istringstream ss(in);
    std::string prefix;
    ss >> prefix;
    if (prefix == "IsMainCam") {
        bool t;
        ss >> t;
        if (t && GetOwner() != nullptr) {
            GetOwner()->GetScene()->SetCurrentCamera(GetOwner());
        }
    }
}

#pragma endregion

#pragma region Script C

void ScriptComponent::LoadScript()
{
    size_t lastSlash = filePath.find_last_of("/\\");
    size_t dot = filePath.find_last_of(".");
    if (lastSlash == std::string::npos) lastSlash = -1;
    if (dot == std::string::npos || dot <= lastSlash) dot = filePath.length();

    ComponentName = filePath.substr(lastSlash + 1, dot - lastSlash - 1);
    
    auto& lua = IE::ScriptingEngine::Get().GetState();
    lua.script_file(filePath);



    m_GlobalVariables.clear();

    for (const auto& kv : lua.globals()) {
        std::string name = kv.first.as<std::string>();
        sol::type t = kv.second.get_type();

        // Ignore standard libs and internal stuff
        if (name[0] == ('_') ||
            name == "math" || name == "string" || name == "table" ||
            name == "io" || name == "os" || name == "coroutine" ||
            name == "debug" || name == "package" || name == "base" ||
            name == "bit32" || name == "utf8")
            continue;

        // You can also filter out functions if you only want variables
        if (t == sol::type::function)
            continue;

        m_GlobalVariables[name] = kv.second;
    }

    lua["self"] = GetOwner();


    // Optional: Extract script-defined functions
    m_OnStart = lua["OnStart"];
    m_OnUpdate = lua["OnUpdate"];
    
    IE_LOG_SUCCESS("Loaded " << filePath <<" successfully ");

}

void ScriptComponent::Update(){
    if (m_OnUpdate.valid()) m_OnUpdate();

}

void ScriptComponent::Render(){
}

void ScriptComponent::Start(){
    if (m_OnStart.valid()) m_OnStart();
}

void ScriptComponent::GuiRender() {

    if (filePath.empty()) {
        // Show drop zone
        ImGui::Text("Drop Lua script here:");
        ImGui::Dummy({ 0.0f, 5.0f });

        ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, 40.0f);
        ImGui::Button("##DropZone", size);

        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE")) {
                std::string droppedPath = (const char*)payload->Data;

                std::string extension = std::filesystem::path(droppedPath).extension().string();
                std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
                if (extension != ".lua") { IE_LOG_ERROR("Couldn't add file with " << extension << " extension. Please use Lua Scripts"); }

                else{
                    filePath = droppedPath;
                    LoadScript(); 
                }
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "Only *.lua files are allowed.");
    }
    else {
        ImGui::Text("Script:");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), filePath.c_str());

        if (ImGui::Button("Reload Script")) {
            LoadScript();
        }

        ImGui::SameLine();

        if (ImGui::Button("Remove Script")) {
            filePath.clear();
            m_OnStart = sol::function();
            m_OnUpdate = sol::function();
        }

        ImGui::Separator();

        for (auto& [name, value] : m_GlobalVariables) {
            ImGui::Text("%s", name.c_str());
        }

    }
}

void ScriptComponent::Serialize(std::ostream& out) {
    out << filePath << std::endl;
}

void ScriptComponent::Deserialize(const std::string& in) {
    filePath = in;
    LoadScript();
}



#pragma endregion

#pragma region Material C

void MaterialComponent::GuiRender()
{
    ImGui::Text("Render Component (Multiple Materials)");

    for (size_t i = 0; i < m_Mats.size(); ++i)
    {
        ImGui::PushID(static_cast<int>(i));

        ImGui::Text("Material %zu", i);

        // --- Vertex Shader Path ---
        char vsBuffer[256];
        strncpy_s(vsBuffer, m_Mats[i].vsPath.c_str(), sizeof(vsBuffer));
        vsBuffer[sizeof(vsBuffer) - 1] = '\0';

        if (ImGui::InputText("VS Path", vsBuffer, sizeof(vsBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Mats[i].vsPath = vsBuffer;
            GetOwner()->GetComponent<RenderComponent>()->ReaplyMaterials();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
            {
                std::string droppedPath = (const char*)payload->Data;
                m_Mats[i].vsPath = droppedPath;
                GetOwner()->GetComponent<RenderComponent>()->ReaplyMaterials();
            }
            ImGui::EndDragDropTarget();
        }

        // --- Fragment Shader Path ---
        char fsBuffer[256];
        strncpy_s(fsBuffer, m_Mats[i].fsPath.c_str(), sizeof(fsBuffer));
        fsBuffer[sizeof(fsBuffer) - 1] = '\0';

        if (ImGui::InputText("FS Path", fsBuffer, sizeof(fsBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_Mats[i].fsPath = fsBuffer;
            GetOwner()->GetComponent<RenderComponent>()->ReaplyMaterials();
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_FILE"))
            {
                std::string droppedPath = (const char*)payload->Data;
                m_Mats[i].fsPath = droppedPath;
                GetOwner()->GetComponent<RenderComponent>()->ReaplyMaterials();
            }
            ImGui::EndDragDropTarget();
        }

        ImGui::Separator();
        ImGui::PopID();
    }

    if (ImGui::Button("Add Material"))
    {
        m_Mats.emplace_back(); // Add empty material
    }
}

#pragma endregion