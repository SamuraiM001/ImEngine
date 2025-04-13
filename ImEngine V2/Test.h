#pragma once
#include <cassert>

#ifdef __cplusplus
#define TEST(f) \
        static void f(void); \
        struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
        static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU",read)
#define INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
#ifdef _WIN64
#define TEST(f) INITIALIZER2_(f,"")
#else
#define TEST(f) INITIALIZER2_(f,"_")
#endif
#else
#define TEST(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
#endif
