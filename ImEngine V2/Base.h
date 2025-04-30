#pragma once
#include <iostream>

#ifdef _DEBUG
#define IE_DEBUGBREAK() __debugbreak()
#endif

#define BIT(x) (1 << x)

#define IE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { \
    return this->fn(std::forward<decltype(args)>(args)...); \
}

// Prevent redefinition of logging macros
#ifndef IE_LOG
#define IE_LOG(x)         std::cout << "IE [LOG]: " << x << "\n"
#define IE_LOG_SUCCESS(x) std::cout << "IE [SUCCESS]: " << x << "\n"
#define IE_LOG_WARN(x)    std::cout << "IE [WARN]: " << x << "\n"
#define IE_LOG_ERROR(x)   std::cerr << "IE [ERROR]: " << x << "\n"
#endif

#ifndef IE_ASSERT
#ifdef _DEBUG
#define IE_ASSERT(x, ...) { \
            if (!(x)) { \
                std::cerr << "Assertion failed: " << #x << "\n"; \
                std::cerr << "File: " << __FILE__ << ":" << __LINE__ << "\n"; \
                std::cerr << "Message: "; \
                fprintf(stderr, __VA_ARGS__); \
                std::cerr << "\n"; \
                IE_DEBUGBREAK(); \
            } \
        }
#else
#define IE_ASSERT(x, ...)
#endif
#endif
