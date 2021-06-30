#include "InputEvent.hpp"

#include <fmt/format.h>

#include "Utils/Visitor.hpp"
#include <type_traits>

namespace Events {

void from_json(const nlohmann::json& j, InputEvent& event) {
    enum class EventType {
        JoinMatchRequest_,
        OfferRequest_,
        PassRequest_,
        AttackRequest_,
        Invalid
    };
    using ET = EventType;  // Clang 12 doesn't support using enum :)

    constexpr static auto toType = [&](std::string_view text) {
        if (text == "joinMatchRequest") {
            return ET::JoinMatchRequest_;
        } else if (text == "offer") {
            return ET::OfferRequest_;
        } else if (text == "pass") {
            return ET::PassRequest_;
        } else if (text == "attack") {
            return ET::AttackRequest_;
        } else
            return ET::Invalid;
    };

    switch (toType(j.at("type").get<std::string_view>())) {
        case ET::JoinMatchRequest_:
            event = JoinMatchRequest{
                .nickname = j.at("nickname"),
                .matchID = j.at("matchID")};
            break;
        case ET::OfferRequest_:
            event = OfferRequest{{}, {{.nickname = {}}, j.at("gold")}};
            break;
        case ET::PassRequest_:
            event = PassRequest{};
            break;
        case ET::AttackRequest_:
            event = AttackRequest{};
            break;
        case ET::Invalid:
            event = Error{fmt::format("{} is not a valid input type", j.at("type").get<std::string_view>())};
            break;
    }
}

[[nodiscard]] InputEvent toInputEvent(std::string_view nickname,
                                      std::string_view message) {
    try {
        const auto j = nlohmann::json::parse(message);
        InputEvent e = j;

        // Assign nickname
        std::visit(visitor{
                    [&]<class T>(T& pe) requires std::is_base_of_v<PlayerEvent, T> { 
                        pe.nickname = nickname; 
                        },
                    [&](const auto& /* do nothing */) {},
                },
                e);
        return e;
    } catch (const nlohmann::json::out_of_range& ex) {
        // Missing key
        return Error{"Missing necessary event key-value"};
    } catch (const nlohmann::json::parse_error& ex) {
        // Malformed JSON
        return Error{"Malformed JSON received"};
    } catch (const std::exception& ex) {
        // Anything I'm missing :)
        return Error{"Error in received message"};
    }
}

}  // namespace Events
