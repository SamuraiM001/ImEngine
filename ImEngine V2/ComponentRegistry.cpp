#include "ComponentRegistry.h"
#include "Components.h"
using namespace IE;

void ComponentRegistry::RegisterComponents(){
    ComponentRegistry::Get().Register<TransformComponent>();
    ComponentRegistry::Get().Register<RenderComponent>();
    ComponentRegistry::Get().Register<CollisionComponent>();
    ComponentRegistry::Get().Register<MovementComponent>();
    ComponentRegistry::Get().Register<CameraComponent>();
    ComponentRegistry::Get().Register<InputComponent>();
    ComponentRegistry::Get().Register<ScriptComponent>();
    ComponentRegistry::Get().Register<MaterialComponent>();
}


