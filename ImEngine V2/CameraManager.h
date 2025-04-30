#pragma once
#include "ECS.h"


namespace IE {
	class CameraManager {
	public:
		CameraManager() {};
		private:
			Object* m_MainCamera;
			std::vector<Object> m_CamContainer;
	};
}

