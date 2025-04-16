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


    static std::string OpenDirectory();

    void LoadDirectory(const std::string directory);
    const std::vector<ResourceEntry>& GetDirectory() const;
    std::string GetCurrentPath();
    void GoBack();
private:
    std::vector<ResourceEntry> m_DirectoryContents;
    std::stack<std::string> m_JoinedPaths;
};
