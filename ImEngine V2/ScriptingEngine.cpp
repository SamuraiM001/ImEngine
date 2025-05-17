#include "ScriptingEngine.h"
#include <sol/sol.hpp>
#include "ECS.h"
#include "Components.h"

using namespace IE;



void ScriptingEngine::Initialize() {
	lua.open_libraries(sol::lib::base, sol::lib::math);

	// Types
	lua.new_usertype<Object>("Object", 
		"name", &Object::m_Name
		,"_id",&Object::m_ID
	);
	lua.new_usertype<Model>("Model",
		"meshes", &Model::meshes,
		"meshCount", &Model::meshCount,
		"meshMaterial", &Model::meshMaterial
	);


	lua.new_usertype<TransformComponent>("TransformComponent",
		"position", &TransformComponent::m_Position,
		"rotation", &TransformComponent::m_Rotation,
		"scale", &TransformComponent::m_Scale
	);

	lua.new_usertype<CameraComponent>("CameraComponent",
		"fov", &CameraComponent::FOV
	);
	lua.new_usertype<RenderComponent>("RenderComponent",
		"model", &RenderComponent::m_Model
	);


	// Lua binding
	lua["Object"]["AddComponent"] = [this](Object& self, const std::string& typeName) -> sol::object {

		if (typeName == "TransformComponent") {
			if (!self.GetComponent<TransformComponent>())
				self.AddComponent<TransformComponent>();
			return sol::make_object(lua, self.GetComponent<TransformComponent>());
		}
		if (typeName == "CameraComponent") {
			if (!self.GetComponent<CameraComponent>())
				self.AddComponent<CameraComponent>();
			return sol::make_object(lua, self.GetComponent<CameraComponent>());
		}
		if (typeName == "RenderComponent") {
			if (!self.GetComponent<RenderComponent>())
				self.AddComponent<RenderComponent>();
			return sol::make_object(lua, self.GetComponent<RenderComponent>());
		}
		
		if (ComponentRegistry::Get().IsScriptComponent(typeName) ) {
			std::cout << "[Lua] Warning: Unknown component type '" << typeName << "'\n";
			return sol::nil;
		}
		return sol::nil;
	};

}



