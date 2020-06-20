#include "Color.hpp"

#include <map>
#include <sstream>
#include <string_view>

using CT = Color::ColorType;
namespace {

constexpr std::string_view colorToStr(CT color) {
    switch (color) {
        case CT::Red:
            return "31";
            break;
        case CT::Blue:
            return "34";
            break;
        case CT::None:
        default:
            return "";
            break;
    }
}

constexpr std::string_view colorStart{"\033["};
constexpr std::string_view colorEnd{"m"};
constexpr std::string_view colorBold{"1"};
constexpr std::string_view colorReset{"0"};
constexpr std::string_view colorSeparator{";"};
}  // namespace

std::string Color::colorString(std::string_view text,
                               ColorType color,
                               bool bold) {
    std::stringstream ss;
    const auto strColor = colorToStr(color);
    if (!bold && strColor.empty()) {
        // Nothing to color
        return std::string(text);
    }
    ss << colorStart;
    if (!strColor.empty()) {
        ss << strColor;
        if (bold) {
            ss << colorSeparator;
        }
    }
    if (bold) {
        ss << colorBold;
    }
    ss << colorEnd << text << colorStart << colorReset << colorEnd;
    return ss.str();
}