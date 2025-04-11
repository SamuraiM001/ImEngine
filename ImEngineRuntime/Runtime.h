#pragma once

#ifdef RUNTIME_EXPORTS
#define RUNTIME_API __declspec(dllexport)
#else
#define RUNTIME_API __declspec(dllimport)
#endif

extern "C" {
    RUNTIME_API bool IsRuntimeRequestingExit();
    RUNTIME_API void InitRuntime(void* windowHandle);  // Pass parent window handle
    RUNTIME_API void TickRuntime(float deltaTime);
    RUNTIME_API void Shutdown();
}