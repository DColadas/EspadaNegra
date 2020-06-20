#include "Time.hpp"

#include <iomanip>
#include <sstream>

std::string Time::timestampToString(Timestamp ts) {
    std::chrono::milliseconds milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(ts.time_since_epoch());
    std::time_t ts_time = std::chrono::system_clock::to_time_t(ts);

    // Convert std::time_t to std::tm
    std::tm tm;
    localtime_r(&ts_time, &tm);

    std::stringstream ss;
    // Output hour
    if (tm.tm_hour <= 9) {
        ss << '0';
    }
    ss << tm.tm_hour << ':';
    // Output minute
    if (tm.tm_min <= 9) {
        ss << '0';
    }
    ss << tm.tm_min << ':';
    // Output seconds with milliseconds
    std::chrono::duration<double> sec = ts -
                                        std::chrono::system_clock::from_time_t(ts_time) +
                                        std::chrono::seconds(tm.tm_sec);
    if (sec.count() < 10) {
        ss << '0';
    }
    ss << std::fixed << sec.count();
    return ss.str();
}
