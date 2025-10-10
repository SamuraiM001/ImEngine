#include "ConnectionManager.h"
#include "curl/curl/curl.h"
#include "nlohmann/json.hpp"
#include <iostream>
#include <Base.h>

using json = nlohmann::json;

size_t ConnectionManager::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    out->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::vector<PluginInfo> ConnectionManager::FetchPlugins(const std::string& url) {
    std::vector<PluginInfo> plugins;

    CURL* curl = curl_easy_init();
    if (!curl)
        return plugins;

    std::string buffer;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        try {
            json data = json::parse(buffer);
            int count = 2;//Item limit that is shown on screen
            for (auto& item : data) {
                if (item["id"].get<int>() > count) break;
                PluginInfo plugin;
                plugin.name = item["name"].get<std::string>();
                plugin.description = item["description"].get<std::string>();
                plugin.version = item["version"].get<std::string>();
                plugins.push_back(plugin);
            }
        }
        catch (...) {}
    }
    else {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    return plugins;
}

void ConnectionManager::FetchPluginsAsync(const std::string& url, std::atomic<bool>& loading, std::vector<PluginInfo>& outPlugins) {
    loading = true;
    std::thread([url, &loading, &outPlugins]() {
        auto plugins = FetchPlugins(url);
        outPlugins = std::move(plugins);
        loading = false;
        }).detach();
}
