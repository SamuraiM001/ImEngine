#include "Components.h"
#include <filesystem>
using namespace IE;


void RenderComponent::GuiRender()
{
    ImGui::Text("Mesh");
    ImGui::PushID("MeshDrop");

    bool clicked = ImGui::Button(m_Model ? m_ModelPath.c_str() : "Drop .obj file here", ImVec2(-1, 32));

    // ✅ Right-click directly on the button to open context menu
    if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        ImGui::OpenPopup("MeshContextMenu");
    }

    // 💧 Drag and drop handler
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

    // ✅ Context menu opened on right-click
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
}


void RenderComponent::Render() {
    if (!m_Model || m_Model->meshCount == 0 || m_Model->meshes[0].vertexCount == 0) {
        m_Model = std::make_shared<Model>(LoadModelFromMesh(GenMeshCube(1, 1, 1)));
    }


    Object& owner = *GetOwner();
    Vector3 rot = owner.m_Rotation;
    Vector3 pos = owner.m_Position;
    Vector3 scale = owner.m_Scale;

    Matrix transform = MatrixMultiply(
        MatrixMultiply(
            MatrixScale(scale.x, scale.y, scale.z),
            MatrixRotateXYZ({
                DEG2RAD * rot.x,
                DEG2RAD * rot.y,
                DEG2RAD * rot.z
                })
        ),
        MatrixTranslate(pos.x, pos.y, pos.z)
    );

    m_Model->transform = transform;
    DrawModel(*m_Model, Vector3Zero(), 1.0f, WHITE);
}