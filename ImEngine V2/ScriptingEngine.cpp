#include "ScriptingEngine.h"
#include <sol/sol.hpp>
#include "ECS.h"
#include "Components.h"

using namespace IE;



void ScriptingEngine::Initialize() {
	lua.open_libraries(sol::lib::base, sol::lib::math);

	lua.new_usertype<Object>("Object",
		"name", &Object::m_Name
	);
	lua.new_usertype<Vector3>("Vector3",
		sol::constructors<Vector3(), Vector3(float, float, float)>(),
		"x", &Vector3::x,
		"y", &Vector3::y,
		"z", &Vector3::z
	);
	lua.new_usertype<TransformComponent>("TransformComponent",
		"position", &TransformComponent::m_Position,
		"rotation", &TransformComponent::m_Rotation,
		"scale", &TransformComponent::m_Scale
	);


	lua["Object"]["AddComponent"] = [](Object& self, const std::string& typeName) -> sol::object {
		auto it = s_AddComponentMap.find(typeName);
		if (it == s_AddComponentMap.end())
			return sol::nil;

		void* raw = it->second(self);

		if (typeName == "TransformComponent")
			return sol::make_object(lua, *(TransformComponent*)raw);

		// etc...

		return sol::nil;
		};



}
