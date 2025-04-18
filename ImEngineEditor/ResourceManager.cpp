#include "ResourceManager.h"
#include "Base.h"
#include <windows.h>
#include <filesystem>
#include <shobjidl_core.h> // Just part of windows.h in modern toolsets


std::string ResourceManager::OpenFile(const std::string& fType)
{
    OPENFILENAMEA ofn;
    CHAR szFile[MAX_PATH] = { 0 };

    // Build proper double-null-terminated filter string
    std::string filterDesc = fType + " Files (*." + fType + ")";
    std::string filterPattern = "*." + fType;

    std::vector<char> filter;
    filter.insert(filter.end(), filterDesc.begin(), filterDesc.end());
    filter.push_back('\0');
    filter.insert(filter.end(), filterPattern.begin(), filterPattern.end());
    filter.push_back('\0'); 

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle =   "Select a File";
    ofn.lpstrFilter = filter.data();
    ofn.nFilterIndex = 0;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if (GetOpenFileNameA(&ofn))
    {
        return ofn.lpstrFile;
    }

    return "";
}



std::string ResourceManager::OpenFolder() {
    std::string result;

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hr)) {
        IFileDialog* pFileDialog = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pFileDialog));

        if (SUCCEEDED(hr)) {
            DWORD dwOptions;
            pFileDialog->GetOptions(&dwOptions);
            pFileDialog->SetOptions(dwOptions | FOS_PICKFOLDERS);

            hr = pFileDialog->Show(NULL);
            if (SUCCEEDED(hr)) {
                IShellItem* pItem = nullptr;
                if (SUCCEEDED(pFileDialog->GetResult(&pItem))) {
                    PWSTR pszFilePath = nullptr;
                    if (SUCCEEDED(pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
                        char path[MAX_PATH];
                        wcstombs(path, pszFilePath, MAX_PATH);
                        result = path;
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileDialog->Release();
        }
        CoUninitialize();
    }

    return result;
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


