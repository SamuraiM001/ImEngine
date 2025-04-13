#pragma once

namespace IE {
	class Scene;

	class SaveManager{
	public:
		 static void SaveSceneToAFile(Scene* Scene);
		 static void LoadSceneFromAFile(Scene* Scene);
	};
}

