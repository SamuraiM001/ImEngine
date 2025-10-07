#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

namespace IE {
	class ShaderRegistry {
	public:

		struct SceneShader {
			Shader m_Shader;
			bool isActive;
		};

		void AddShader(std::string name, std::string pathvs,std::string pathfs);

		void RemoveShader(std::string name);

		std::unordered_map<std::string, SceneShader>* GetShaders() { return &m_Shaders; }

		void BeginShading();

		void EndShading();
	private:
		std::unordered_map<std::string, SceneShader> m_Shaders;
	};
}