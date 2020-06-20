#pragma once

#include <string>
#include <string_view>

namespace Color {
enum class ColorType {
    None,
    Red,
    Blue
};

// Format the input text according to the parameters
std::string colorString(std::string_view text,
                        ColorType color,
                        bool bold);
}  // namespace Color