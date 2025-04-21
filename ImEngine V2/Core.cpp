#include "Core.h"
#include <fstream>

using namespace IE;

std::string IE::Core::m_WorkFolder = "";
std::string IE::Core::m_ProjectName = "";
std::string IE::Core::m_StartScene = "";
std::string IE::Core::m_ProjectFile = "";

void Core::ParseProjectFile(std::string folder)
{
    std::string projectFilePath = m_ProjectFile;
    std::ifstream file(projectFilePath);

    if (!file.is_open()) {
        IE_LOG_ERROR("Failed to open project file at: " + projectFilePath);
        return;
    }

    std::string line;
    bool foundStartupScene = false;
    bool foundProjectName = false;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.rfind("ProjectName=", 0) == 0) {
            m_ProjectName = line.substr(std::string("ProjectName=").length());
            IE_LOG_SUCCESS("Project Name : " + m_ProjectName);
            foundProjectName = true;
        }
        else if (line.rfind("StartupScene=", 0) == 0) {
            m_StartScene = line.substr(std::string("StartupScene=").length());
            IE_LOG_SUCCESS("Startup Scene found: " + m_StartScene);
            foundStartupScene = true;
            break;
        }
       
    }

    if (!foundStartupScene || !foundProjectName) {
        IE_LOG_ERROR("Project Has Missing Data. Edit the .improject file to fix it");
    }


}

void Core::WriteProjectFile()
{
    if (m_ProjectFile.empty()) {
        IE_LOG_ERROR("Cannot write project file: path is empty");
        return;
    }

    std::ofstream file(m_ProjectFile, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        IE_LOG_ERROR("Failed to open project file for writing: " + m_ProjectFile);
        return;
    }

    file << "ProjectName=" << m_ProjectName << "\n";
    file << "StartupScene=" << m_StartScene << "\n";

    IE_LOG_SUCCESS("Project file written to: " + m_ProjectFile);
    file.close();
}

void Core::Initialize(int argc, char* argv[]) {
    if (argc > 1) {
        std::string fullPath = argv[1];
        m_ProjectFile = argv[1];
        IE_LOG(argv[1]);
        size_t lastSlash = fullPath.find_last_of("\\");
        if (lastSlash != std::string::npos && lastSlash != fullPath.size()) {
            m_WorkFolder = fullPath.substr(0, lastSlash);
            IE_LOG("Work folder set to: " + m_WorkFolder);
            ParseProjectFile(m_WorkFolder);
        }
        else {
            IE_LOG_ERROR("Invalid path provided, cannot determine work folder. Open the project or Engine will not work!");
        }
    }
    else {
        IE_LOG_ERROR("No Project Path is found");
    }
}

void Core::Shutdown(){
    WriteProjectFile();

}
