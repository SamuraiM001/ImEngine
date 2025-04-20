#pragma once

#ifdef RUNTIME_EXPORTS
#define RUNTIME_API __declspec(dllexport)
#else
#define RUNTIME_API __declspec(dllimport)
#endif
#include "ImEngine.h"

extern "C" {
    RUNTIME_API bool IsRuntimeRequestingExit();
    RUNTIME_API void InitRuntime(std::string ProjectPath);  // Pass parent window handle
    RUNTIME_API void TickRuntime(float deltaTime);
    RUNTIME_API void Shutdown();
}

std::string m_ProjectPath;

IE::Scene m_Scene;

