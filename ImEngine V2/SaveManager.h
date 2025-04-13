#pragma once
#include <string>

namespace IE {
	class Scene;

	class SaveManager{
	public:
		 static void SaveSceneToAFile(Scene* Scene);
		 static void LoadSceneFromAFile(Scene* Scene,std::string FilePath);
	};
}

