#include "Profiler.h"

Profiler& Profiler::Get() {
    static Profiler instance;
    return instance;
}

void Profiler::BeginFrame() {
    m_Timings.clear();
}

void Profiler::Begin(const std::string& name) {
    m_StartTimes[name] = std::chrono::high_resolution_clock::now();
}

void Profiler::End(const std::string& name) {
    auto end = std::chrono::high_resolution_clock::now();
    auto start = m_StartTimes[name];
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    TimingInfo& info = m_Timings[name];
    info.totalTime += duration;
    info.callCount++;
}
