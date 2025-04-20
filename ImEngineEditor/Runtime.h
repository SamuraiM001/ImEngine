#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NOUSER
#include <windows.h>
#undef near
#undef far
#undef ERROR

#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
#include <iostream>
#include <raylib.h>

class RuntimeManager {
public:
    RuntimeManager();
    ~RuntimeManager();

    void Load();
    void Stop();
    void Unload();
    void Start(void* parentWindow = nullptr);
    void MT(void* PW);
    bool IsRunning() const;

private:

    bool isInitialized = false;
    HMODULE m_DLL = nullptr;

    std::condition_variable m_CV;
    std::thread m_RuntimeThread;
    std::atomic<bool> m_Running{ false };
    std::atomic<bool> m_ShouldRunTick{ false };
    std::atomic<bool> m_Initialized{ false };
    std::atomic<bool> m_ShouldClose = false;
    std::mutex m_Mutex;

    // Function pointers
    void (*m_Init)(std::string) = nullptr;
    void (*m_Tick)(float) = nullptr;
    void (*m_Shutdown)() = nullptr;
    bool (*m_IsRequestingExit)() = nullptr;

    // Window management
    void* m_ParentWindow = nullptr;
    bool m_OwnsWindow = false;
};

