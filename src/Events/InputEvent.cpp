#include "InputEvent.hpp"

#include "Utils/Visitor.hpp"

namespace Events {

void from_json(const nlohmann::json& j, InputEvent& event) {
    enum class EventType {
        JoinMatchRequest_,
        OfferRequest_,
        PassRequest_,
        AttackRequest_,
        Invalid
    };

    using enum EventType;
    constexpr static auto toType = [&](std::string_view text) {
        if (text == "joinMatchRequest") {
            return JoinMatchRequest_;
        } else if (text == "offer") {
            return OfferRequest_;
        } else if (text == "pass") {
            return PassRequest_;
        } else if (text == "attack") {
            return AttackRequest_;
        } else
            return Invalid;
    };

    switch (toType(j.at("type").get<std::string_view>())) {
        case JoinMatchRequest_:
            event = JoinMatchRequest{
                .nickname = j.at("nickname"),
                .matchID = j.at("matchID")};
            break;
        case OfferRequest_:
            event = OfferRequest{{.nickname = {}},
                                 j.at("gold")};
            break;
        case PassRequest_:
            event = PassRequest{{}};
            break;
        case AttackRequest_:
            event = AttackRequest{{}};
            break;
        case Invalid:
            event = {};
            break;
    }
}

[[nodiscard]] InputEvent toInputEvent(std::string_view nickname,
                                      std::string_view message) {
    const auto j = nlohmann::json::parse(message);
    InputEvent e = j;

    // Assign nickname
    std::visit(visitor{
                   [&](InMatchInputEvent& ime) { ime.nickname = nickname; },
                   [&](const auto& /* do nothing */) {},
               },
               e);
    return e;
}

}  // namespace Events
