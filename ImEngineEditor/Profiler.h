#pragma once
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <string>

class Profiler
{
public:
    struct TimingInfo {
        long long totalTime = 0;
        int callCount = 0;
    };

    static Profiler& Get() {
        static Profiler instance;
        return instance;
    }

    void BeginFrame() {
        m_Timings.clear();
    }

    void Begin(const std::string& name) {
        m_StartTimes[name] = std::chrono::high_resolution_clock::now();
    }

    void End(const std::string& name) {
        auto end = std::chrono::high_resolution_clock::now();
        auto start = m_StartTimes[name];
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        TimingInfo& info = m_Timings[name];
        info.totalTime += duration;
        info.callCount++;
    }

    const std::unordered_map<std::string, TimingInfo>& GetTimings() const {
        return m_Timings;
    }

private:
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> m_StartTimes;
    std::unordered_map<std::string, TimingInfo> m_Timings;
};
