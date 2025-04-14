#include "ComponentRegistry.h"
#include "Components.h"
using namespace IE;

void ComponentRegistry::RegisterComponents(){
    IE::ComponentRegistry::Get().Register<RenderComponent>();
}


