#include "ShaderRegistry.h"
#include "Base.h"


void IE::ShaderRegistry::AddShader(std::string name, std::string pathvs,std::string pathfs) {
	m_Shaders[name].m_Shader = LoadShader(pathvs.c_str(),pathfs.c_str());	
}

void IE::ShaderRegistry::RemoveShader(std::string name) {
	m_Shaders.erase(name);
}

void IE::ShaderRegistry::BeginShading(){
	for (auto& [x, y] : m_Shaders) {
		if (y.isActive) {
			BeginShaderMode(y.m_Shader);
		}
	}
}
void IE::ShaderRegistry::EndShading() {
	for (auto& [x, y] : m_Shaders) {
		if (y.isActive)
		EndShaderMode();
	}
}
