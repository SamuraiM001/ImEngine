#pragma once
#include <iostream>
#include <unordered_map>
#include <chrono>
#include <string>

#define START_PROFILING(x) Profiler::Get().Begin(x);

#define END_PROFILING(x) Profiler::Get().End(x);

class Profiler
{
public:
    struct TimingInfo {
        long long totalTime = 0;
        int callCount = 0;
    };

    static Profiler& Get();

    void BeginFrame();

    void Begin(const std::string& name);

    void End(const std::string& name);

    const std::unordered_map<std::string, TimingInfo>& GetTimings() const {
        return m_Timings;
    }

private:
    std::unordered_map<std::string, std::chrono::high_resolution_clock::time_point> m_StartTimes;
    std::unordered_map<std::string, TimingInfo> m_Timings;
};
