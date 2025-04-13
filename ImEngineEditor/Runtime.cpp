#include "Runtime.h"

RuntimeManager::RuntimeManager() {
    Load();
}

RuntimeManager::~RuntimeManager() {
    Stop();    
    Unload();
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

    m_Running = true;
    m_RuntimeThread = std::thread([this]() {
        while (m_Running) {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_CV.wait(lock, [this]() { return m_ShouldRunTick || !m_Running; });

            if (!m_Running) break;

            if (!m_Initialized && m_Init) {
                m_Init(nullptr); // you can store window handle and pass it here
                m_Initialized = true;
            }

            lock.unlock();

            while (m_ShouldRunTick && m_Running) {
                {
                    std::lock_guard<std::mutex> tickLock(m_Mutex);
                    if (m_Tick) m_Tick(GetFrameTime());
                    if (m_IsRequestingExit && m_IsRequestingExit()) {
                        m_ShouldRunTick = false;
                    }
                }
            }

            lock.lock();
            if (m_Initialized && m_Shutdown) {
                m_Shutdown();
                m_Initialized = false;
            }
        }
        });
}


void RuntimeManager::Unload() {
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_Running = false;
    }
    m_CV.notify_one();

    if (m_RuntimeThread.joinable())
        m_RuntimeThread.join();

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
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (!m_Running || m_ShouldRunTick) return;
        m_ShouldRunTick = true;
    }
    m_CV.notify_one(); // wake the thread
}


void RuntimeManager::Stop() {
    m_ShouldRunTick = false;

    std::lock_guard<std::mutex> lock(m_Mutex);
    if (m_Initialized && m_Shutdown) {
        m_Shutdown();
        m_Initialized = false;
    }
}



void RuntimeManager::MT(void* PW) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    if (m_Init) m_Init(PW);
}

bool RuntimeManager::IsRunning() const {
    return m_Running.load();
}
