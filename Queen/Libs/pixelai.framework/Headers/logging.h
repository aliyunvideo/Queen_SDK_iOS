#ifndef LOGGING_H
#define LOGGING_H
#include <iostream>

#if defined(_DEBUG) || defined(DEBUG)
#define RACE_DEBUG 1
#endif // _DEBUG


#define PIXELAI_PLAT_UNKNOWN            0
#define PIXELAI_PLAT_IOS                1
#define PIXELAI_PLAT_ANDROID            2
#define PIXELAI_PLAT_WINDOWS            3
#define PIXELAI_PLAT_MAC                4
#define PIXELAI_PLAT_LINUX              5
/*******---------------------*************/
// Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID)
#define PIXELAI_APPLE 1
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define PIXELAI_TARGET_PLAT         PIXELAI_PLAT_IOS
#elif TARGET_OS_MAC
#define PIXELAI_TARGET_PLAT         PIXELAI_PLAT_MAC
#endif
#endif

#if defined(ANDROID)
#define PIXELAI_TARGET_PLAT         PIXELAI_PLAT_ANDROID
#endif

// win32
#if defined(_WIN32) //&& defined(_WINDOWS)
#define PIXELAI_TARGET_PLAT         PIXELAI_PLAT_WINDOWS
#endif

// linux
#if defined(LINUX) && !defined(__APPLE__)
#define PIXELAI_TARGET_PLAT         PIXELAI_PLAT_LINUX
#endif

#ifndef PIXELAI_TARGET_PLAT
#define PIXELAI_TARGET_PLAT             PIXELAI_PLAT_UNKNOWN
#endif
/*******---------------------*************/

enum LogLevel
{
    kVerbose = 2,
    kDebug,
    kInfo,
    kWarn,
    kError,
    kFatal,
};


/**
 * @brief Set output log level
 * Default log level:
 * Debug: VERBOSE
 * Release: WARN
 */
void SetLogLevel(LogLevel level);

/**
 * @brief Set output file
 * @param fp the new log file pointer
 * @return the old log file pointer, default file is stderr
 */
FILE* SetLogFilePointer(FILE* fp);

typedef void (*PFN_LogCallBack)(int level,
                                const char* tag,
                                int mask,
                                const char* fname,
                                const int line,
                                const char* func,
                                int64_t logId,
                                const char* fmt, ...);
void SetLogCallback(PFN_LogCallBack cb);


class  LogMessage
{
public:
    LogMessage(const char* fname, int line, LogLevel severity);
    ~LogMessage();

    void operator()(const char* fmt, ...);

private:
    const char* mFilename;
    int mLine;
    LogLevel mSeverity;
};

template <typename T, int L>
constexpr char const* ShortName(T const (&a)[L], int K = 0)
{
#if defined(_WIN32) || defined(_WIN64)
    return K >= L ? a : a[L - K - 1] == T('\\') ? &a[L - K] : ShortName(a, K + 1);
#else
    return K >= L ? a : a[L - K - 1] == T('/') ? &a[L - K] : ShortName(a, K + 1);
#endif
}


#define PLOGV(fmt, ...) LogMessage(ShortName(__FILE__), __LINE__, kVerbose)(fmt, ##__VA_ARGS__)
#define PLOGD(fmt, ...) LogMessage(ShortName(__FILE__), __LINE__, kDebug)(fmt, ##__VA_ARGS__)
#define PLOGI(fmt, ...) LogMessage(ShortName(__FILE__), __LINE__, kInfo)(fmt, ##__VA_ARGS__)
#define PLOGW(fmt, ...) LogMessage(ShortName(__FILE__), __LINE__, kWarn)(fmt, ##__VA_ARGS__)
#define PLOGE(fmt, ...) LogMessage(ShortName(__FILE__), __LINE__, kError)(fmt, ##__VA_ARGS__)
#define PLOGF(fmt, ...) LogMessage(ShortName(__FILE__), __LINE__, kFatal)(fmt, ##__VA_ARGS__)

#endif // RACE_LOGGING_H
