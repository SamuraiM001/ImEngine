#pragma once
#include <string>
#include <vector>
#include <stack>

class ResourceManager {
public:
    enum FileType {
        Dir,
        File
    };

    struct ResourceEntry {
        std::string name;
        std::string fullPath;
        FileType type;
    };




     static std::string OpenFile(const std::string& fType);
     static std::string OpenFolder();


     //static void CreateFolder();
    // static void CreateAsset(std::string name,std::string dir);

     void ReloadFolder();

     void CreateDir(std::string name);
     void CreateAsset(std::string name);

    void LoadDirectory(const std::string directory);
    void OpenDirectory(const std::string directory);

     std::vector<ResourceEntry>& GetDirectory();
    std::string GetCurrentPath();
    void GoBack();
private:
    std::vector<ResourceEntry> m_DirectoryContents;
    std::stack<std::string> m_JoinedPaths;
};
