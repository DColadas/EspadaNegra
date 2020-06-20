#pragma once

#include <chrono>
#include <string>

using Clock = std::chrono::system_clock;
using Timestamp = Clock::time_point;

namespace Time {
// Returns string of format HH:MM:SS.mmmmmm
std::string timestampToString(Timestamp ts);
}  // namespace Time