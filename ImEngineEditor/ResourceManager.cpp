#include "ResourceManager.h"
#include "Base.h"
#include <windows.h>
#include <filesystem>
#include <shobjidl_core.h> // Just part of windows.h in modern toolsets
#include <fstream>

#ifdef RELEASE
#define _CRT_SECURE_NO_WARNINGS
#endif // RELEASE


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
                        size_t convertedChars = 0;

                        wcstombs_s(&convertedChars, path, MAX_PATH, pszFilePath, _TRUNCATE);

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

void ResourceManager::ReloadFolder(){
    OpenDirectory(GetCurrentPath());
}



void ResourceManager::CreateDir(std::string name){
    std::filesystem::path filePath = std::filesystem::path(GetCurrentPath()) / name;


    if (std::filesystem::create_directory(std::filesystem::path(filePath))) {
        IE_LOG("Directory '" << filePath << "' created successfully.");
    }
    else {
        // This could mean the directory already exists or there was an error
        if (std::filesystem::exists(filePath) && std::filesystem::is_directory(filePath)) {
            std::cout << "Directory '" << filePath << "' already exists." << std::endl;
        }
        else {
            std::cerr << "Error creating directory '" << filePath << "'." << std::endl;
        }
    }
    ReloadFolder();
}

void ResourceManager::CreateAsset(std::string name) {

    // Return if the name is empty or contains invalid characters
    if (name.empty()) return;

    // Build the full file path
    std::filesystem::path filePath = std::filesystem::path(GetCurrentPath()) / name;

    // Avoid throwing exceptions, use error_code instead
    std::error_code ec;
    if (exists(filePath, ec)) {
        IE_LOG_ERROR("File already exists: " << filePath);
        return;
    }

    // Try to create the file and write a basic header
    std::ofstream file(filePath);
    if (!file.is_open()) {
        IE_LOG_ERROR("Could not create file: " << filePath);
        return;
    }

    file.close();

    // Notify or update UI
    ReloadFolder();
}



void ResourceManager::LoadDirectory(const std::string directory) {
    m_JoinedPaths.push(directory);
    OpenDirectory(directory);
}

void ResourceManager::OpenDirectory(const std::string directory) {
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
 std::vector<ResourceManager::ResourceEntry>& ResourceManager::GetDirectory() {
    return m_DirectoryContents;
}

std::string ResourceManager::GetCurrentPath(){
    return m_JoinedPaths.top();
}

void ResourceManager::GoBack(){
    if (m_JoinedPaths.size() > 1) {
        m_JoinedPaths.pop();
        OpenDirectory(m_JoinedPaths.top());
    }
}   


