
// Определение операционной системы
#if defined(_WIN32) || defined(_WIN64)
    #define OS_NAME "Windows"
#elif defined(__APPLE__) && defined(__MACH__)
    #define OS_NAME "macOS"
#elif defined(__linux__)
    #define OS_NAME "Linux"
#elif defined(__FreeBSD__)
    #define OS_NAME "FreeBSD"
#else
    #define OS_NAME "Unknown"
#endif

