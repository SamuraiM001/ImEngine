#include "ComponentRegistry.h"
#include "Components.h"
using namespace IE;

void ComponentRegistry::RegisterComponents(){
    ComponentRegistry::Get().Register<RenderComponent>();
    ComponentRegistry::Get().Register<CollisionComponent>();
    ComponentRegistry::Get().Register<MovementComponent>();
}


