#pragma once
#include <sol/sol.hpp>

namespace IE {
	class ScriptingEngine {
	public:
		static ScriptingEngine& Get() {
			static ScriptingEngine instance;
			return instance;
		}

		void Initialize();

		sol::state& GetState() { return lua; }

	private:
		ScriptingEngine() = default;
		sol::state lua;
	};
}
