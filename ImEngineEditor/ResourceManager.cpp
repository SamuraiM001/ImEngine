#include "ResourceManager.h"
#include "Base.h"
#include <windows.h>
#include <filesystem>



std::string ResourceManager::OpenDirectory()
{
    OPENFILENAMEA ofn;
    CHAR szFile[MAX_PATH] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Select a folder (click on a file inside)";
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
    {
        std::string fullPath = ofn.lpstrFile;
        return fullPath;
    }

    return ""; 
}


void ResourceManager::LoadDirectory(const std::string directory) {
    m_JoinedPaths.push(directory);
    LoadDir(directory);
}

void ResourceManager::LoadDir(const std::string directory) {
    m_DirectoryContents.clear();
    std::filesystem::path dirPath(directory);

    if (!std::filesystem::exists(dirPath)) {
        IE_LOG_ERROR(" [LoadDirectory] Path does not exist: " << directory);
        return;
    }

    if (!std::filesystem::is_directory(dirPath)) {
        IE_LOG_ERROR(" [LoadDirectory] Not a directory: " << directory);
        return;
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            ResourceEntry res;
            res.fullPath = entry.path().string();
            res.name = entry.path().filename().string();
            res.type = entry.is_directory() ? Dir : File;

            m_DirectoryContents.push_back(res);
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        IE_LOG_ERROR("[LoadDirectory] Filesystem error: " << e.what());
    }
}

const std::vector<ResourceManager::ResourceEntry>& ResourceManager::GetDirectory() const{
    return m_DirectoryContents;
}

std::string ResourceManager::GetCurrentPath(){
    return m_JoinedPaths.top();
}

void ResourceManager::GoBack(){
    if (m_JoinedPaths.size() > 1) {
        m_JoinedPaths.pop();
        LoadDir(m_JoinedPaths.top());
    }
    IE_LOG(m_JoinedPaths.size());
}   


