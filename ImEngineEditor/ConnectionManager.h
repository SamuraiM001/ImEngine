#pragma once
#include <string>
#include <vector>
#include <thread>
#include <atomic>

struct PluginInfo {
    std::string name;
    std::string description;
    std::string version;
};

class ConnectionManager {
public:
    // Fetch plugins (blocking)
    static std::vector<PluginInfo> FetchPlugins(const std::string& url);

    // Fetch asynchronously in another thread
    static void FetchPluginsAsync(const std::string& url, std::atomic<bool>& loading, std::vector<PluginInfo>& outPlugins);

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out);
};
