#include "Components.h"
#include <filesystem>

using namespace IE;



#pragma region Transform C
Vector3 TransformComponent::GetForwardVector() {
    Matrix rotationMatrix = MatrixRotateXYZ(m_Rotation);
    return {
        rotationMatrix.m8,
        rotationMatrix.m9,
        rotationMatrix.m10
    };
}\

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
    ImGui::Separator();
    ImGui::Text("Transform");

    ImGui::DragFloat3("Position", &m_Position.x, 0.1f);
    ImGui::DragFloat3("Rotation", &m_Rotation.x, 0.1f);
    ImGui::DragFloat3("Scale", &m_Scale.x, 0.1f);


}

Matrix TransformComponent::GetWorldTransform()
{
    if (GetOwner() != nullptr && GetOwner()->GetParent())
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

#pragma endregion

#pragma region Render C
void RenderComponent::OnAttach(){
    if (GetOwner()->GetComponent<TransformComponent>() == nullptr) {
        GetOwner()->AddComponent<TransformComponent>();
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

                if (extension == ".obj")
                {
                    m_ModelPath = droppedPath;
                    Model rawModel = LoadModel(m_ModelPath.c_str());
                    if (rawModel.meshCount == 0) {
                        IE_LOG_ERROR("Failed to load model: {}", m_ModelPath);
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
                    IE_LOG_ERROR("Failed to reload model: {}", m_ModelPath);
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
#pragma endregion
