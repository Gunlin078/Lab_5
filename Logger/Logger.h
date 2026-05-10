#pragma once

#include <string>

#ifdef _WIN32
#ifdef LOGGER_EXPORTS
#define LOGGER_API __declspec(dllexport) //Если нахожусь внутри библиотеки, то отдавай
#else
#define LOGGER_API __declspec(dllimport) //Если не в библиотеке, то принимай
#endif

#else
#define LOGGER_API
#endif

enum class LogLevel { // Видеть вместо названий значения(0-6)
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

class LOGGER_API Logger {
public:
    static void trace(const std::string& msg);
    static void debug(const std::string& msg);
    static void info(const std::string& msg);
    static void warning(const std::string& msg);
    static void error(const std::string& msg, const char* file, int line);
    static void fatal(const std::string& msg, const char* file, int line);
    static void close();

private:
    static void log(LogLevel level, const std::string& msg, const char* file = "", int line = 0);
};

#ifdef NDEBUG //Если версия не является дебагом, то выполнить 1ю часть (не показывать детальную трассировку и отладочную инфу)

#define LOG_TRACE(msg)
#define LOG_DEBUG(msg)

#else //Если дебаг
#define LOG_TRACE(msg) Logger::trace(msg)
#define LOG_DEBUG(msg) Logger::debug(msg)
#endif
#define LOG_INFO(msg) Logger::info(msg)
#define LOG_WARNING(msg) Logger::warning(msg)
#define LOG_ERROR(msg) Logger::error(msg, __FILE__, __LINE__)
#define LOG_FATAL(msg) Logger::fatal(msg, __FILE__, __LINE__)
