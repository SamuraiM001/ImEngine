#include "Runtime.h"
RuntimeManager::RuntimeManager() {
    Load();
}

RuntimeManager::~RuntimeManager() {
}

void RuntimeManager::Load() {
    if (m_DLL) return;

    std::lock_guard<std::mutex> lock(m_Mutex);
    m_DLL = LoadLibraryA("ImEngineRuntime.dll");
    if (!m_DLL) {
        std::cerr << "Failed to load runtime DLL!" << std::endl;
        return;
    }

    auto getProc = [&](const char* name) -> void* {
        void* ptr = reinterpret_cast<void*>(GetProcAddress(m_DLL, name));
        if (!ptr) std::cerr << "Failed to get " << name << std::endl;
        return ptr;
        };

    m_Init = reinterpret_cast<void(*)(void*)>(getProc("InitRuntime"));
    m_Tick = reinterpret_cast<void(*)(float)>(getProc("TickRuntime"));
    m_Shutdown = reinterpret_cast<void(*)()>(getProc("Shutdown"));
    m_IsRequestingExit = reinterpret_cast<bool(*)()>(getProc("IsRuntimeRequestingExit"));
}

void RuntimeManager::Stop() {
    if (!m_Running.exchange(false)) return;

    m_RuntimeThread.join();
}

void RuntimeManager::Unload() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    if (m_DLL) {
        FreeLibrary(m_DLL);
        m_DLL = nullptr;
        m_Init = nullptr;
        m_Tick = nullptr;
        m_Shutdown = nullptr;
        m_IsRequestingExit = nullptr;
    }
}



void RuntimeManager::Start(void* parentWindow) {

    if (m_Running || !m_DLL) return;

    m_Running = true;
    m_RuntimeThread = std::thread([this, parentWindow]() {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_Init) m_Init(parentWindow);
        }

        while (m_Running) {
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                if (m_Tick) {
                    m_Tick(GetFrameTime());
                }

                if (m_IsRequestingExit && m_IsRequestingExit()) {
                    m_Running = false;
                }
            }

        }
        Stop();
        }); 
}

bool RuntimeManager::IsRunning() const {
    return m_Running.load();
}