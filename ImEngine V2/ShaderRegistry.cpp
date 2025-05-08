#include "ShaderRegistry.h"



void IE::ShaderRegistry::AddShader(std::string name, std::string pathvs,std::string pathfs) {
	m_Shaders[name] = LoadShader(pathvs.c_str(),pathfs.c_str());	
}
void IE::ShaderRegistry::BeginShading(){
	for (auto& [x, y] : m_Shaders) {
		BeginShaderMode(y);
	}
}
void IE::ShaderRegistry::EndShading() {
	for (auto& [x, y] : m_Shaders) {
		EndShaderMode();
	}
}
