#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

namespace IE {
	class ShaderRegistry {
	public:
		void AddShader(std::string name, std::string pathvs,std::string pathfs);

		void RemoveShader(std::string name);

		void BeginShading();

		void EndShading();
	private:
		std::unordered_map<std::string, Shader> m_Shaders;
	};
}