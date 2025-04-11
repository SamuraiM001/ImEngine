#pragma once

#ifdef _DEBUG
	#define IE_DEBUGBREAK() __debugbreak()
#endif

#define BIT(x) (1 << x)

#define IE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define IE_LOG(x)         std::cout << "IE [LOG]: " << x << "\n"  
#define IE_LOG_WARN(x)    std::cout << "IE [WARN]: " << x << "\n"  
#define IE_LOG_ERROR(x)   std::cerr << "IE [ERROR]: " << x << "\n"  

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