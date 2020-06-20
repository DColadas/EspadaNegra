#include "Logger.hpp"

#include <iostream>

#include "Utils/Color.hpp"

namespace {
using LL = Logger::LogLevel;
using CT = Color::ColorType;

constexpr std::string_view levelToString(Logger::LogLevel level) {
    switch (level) {
        case LL::Panic:
            return "pnc";
            break;
        case LL::Error:
            return "err";
            break;
        case LL::Info:
            return "inf";
            break;
        case LL::Debug:
            return "dbg";
            break;
        case LL::Trace:
            return "trc";
            break;
        case LL::None:  // Should never be displayed
        default:        //Not implemented
            return "???";
    }
}

constexpr Color::ColorType logLevelToColor(Logger::LogLevel level) {
    switch (level) {
        case LL::Panic:
            return CT::Red;
            break;
        case LL::Error:
            return CT::Red;
            break;
        case LL::Info:
            return CT::Blue;
            break;
        case LL::Debug:
            return CT::None;
            break;
        case LL::Trace:
            return CT::None;
            break;
        case LL::None:  // Should never be displayed
        default:        //Not implemented
            return CT::None;
    }
}

constexpr bool logLevelToBold(Logger::LogLevel level) {
    switch (level) {
        case LL::Panic:
            return true;
            break;
        case LL::Error:
            return true;
            break;
        case LL::Info:
            return true;
            break;
        case LL::Debug:
            return true;
            break;
        case LL::Trace:
            return false;
            break;
        case LL::None:  // Should never be displayed
        default:        //Not implemented
            return false;
    }
}

}  // namespace

Logger::Logger() {}

void Logger::setLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex);
    logLevel = level;
}

void Logger::enableColors(bool enabled) {
    std::lock_guard<std::mutex> lock(mutex);
    colorsEnabled = enabled;
}

void Logger::print(Timestamp ts, std::string_view file, int line, LogLevel level, std::string_view message) {
    std::lock_guard<std::mutex> lock(mutex);
    if (logLevel == LogLevel::None ||
        level == LogLevel::None ||
        level > logLevel) {
        return;
    }
    const auto strTime = Time::timestampToString(ts);
    const auto strLevel = levelToString(level);

    if (colorsEnabled) {
        const auto coloredLevel = Color::colorString(strLevel, logLevelToColor(level), logLevelToBold(level));
        const auto coloredMessage = Color::colorString(message, logLevelToColor(level), logLevelToBold(level));
        std::cerr << strTime << " [" << coloredLevel << "] " << file << ":" << std::to_string(line) << ": " << coloredMessage << '\n';
    } else {
        std::cerr << strTime << " [" << strLevel << "] " << file << ":" << std::to_string(line) << ": " << message << '\n';
    }
    // Terminate program if there is a panic
    if (level == LogLevel::Panic) {
        abort();
    }
}
