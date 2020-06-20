#pragma once

#include <mutex>
#include <string_view>

#include "Utils/Time.hpp"

#define LOG(level, message) Logger::get().print(Clock::now(), __FILE__, __LINE__, level, message)
#define LOG_PANIC(message) LOG(Logger::LogLevel::Panic, message)
#define LOG_ERROR(message) LOG(Logger::LogLevel::Error, message)
#define LOG_INFO(message) LOG(Logger::LogLevel::Info, message)
#define LOG_DEBUG(message) LOG(Logger::LogLevel::Debug, message)
#define LOG_TRACE(message) LOG(Logger::LogLevel::Trace, message)

class Logger {
   public:
    enum class LogLevel {
        None,   // Disable logging
        Panic,  // Exit program if happens
        Error,  // Error
        Info,   // Relevant information
        Debug,  // Debug messages
        Trace   // Very descriptive debug messages
    };

   private:
    LogLevel logLevel = LogLevel::Info;
    bool colorsEnabled = true;
    std::mutex mutex;

    Logger();

   public:
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    // Get unique instance of Logger
    static Logger& get() {
        static Logger instance;
        return instance;
    }

    // Sets log level (messages of lower severity are ignored)
    void setLevel(LogLevel level);

    // Show colored log messages or not
    void enableColors(bool enabled);

    // Prints the message with the selected severity level
    void print(Timestamp ts, std::string_view file, int line, LogLevel level, std::string_view message);
};
