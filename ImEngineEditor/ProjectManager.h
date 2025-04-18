#pragma once
#include "ImEngine.h"
class ProjectManager
{
public:
	std::string GetProjectPath();
	void LoadProjects();

protected:
	std::vector<std::string> projectPaths;
};

