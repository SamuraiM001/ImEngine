#include "Components.h"
using namespace IE;


void IE::RenderComponent::GuiRender(){
    ImGui::Button("SS");
}

void RenderComponent::Render() {
    static Model cubeModel = LoadModelFromMesh(GenMeshCube(1, 1, 1));

    Object& owner = *GetOwner();
    Vector3 rot = owner.m_Rotation;
    Vector3 pos = owner.m_Position;
    Vector3 scale = owner.m_Scale;

    Matrix transform = MatrixMultiply(
        MatrixMultiply(
            MatrixScale(scale.x, scale.y, scale.z),
            MatrixRotateXYZ( {
            DEG2RAD* rot.x,
            DEG2RAD* rot.y,
            DEG2RAD* rot.z
    })
        ),
        MatrixTranslate(pos.x, pos.y, pos.z)
    );

    cubeModel.transform = transform;
    DrawModel(cubeModel, Vector3Zero(), 1.0f, WHITE);
}
