#include "OutputEvent.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <variant>
#include <vector>

#include "Events/Common.hpp"
#include "Events/InputEvent.hpp"
#include "Model/Card.hpp"
#include "Model/Deck.hpp"
#include "Model/MatchConfig.hpp"
#include "Model/Player.hpp"
#include "Utils/Visitor.hpp"

namespace Events {

void to_json(nlohmann::json& j, const AttackResult& event) {
    j = nlohmann::json{{"type", "attack"},
                       {"nickname", event.nickname}};
}

void to_json(nlohmann::json& j, const Complex& event) {
    j = nlohmann::json{{"type", "complex"},
                       {"events", event.events}};
}

void to_json(nlohmann::json& j, const Draw& event) {
    j = nlohmann::json{{"type", "draw"},
                       {"card", event.card}};
}

void to_json(nlohmann::json& j, const Earn& event) {
    j = nlohmann::json{{"type", "earn"},
                       {"nickname", event.nickname},
                       {"gold", event.gold}};
}

void to_json(nlohmann::json& j, const Error& event) {
    j = nlohmann::json{{"type", "error"},
                       {"message", event.message}};
}

void to_json(nlohmann::json& j, const GetCard& event) {
    j = nlohmann::json{{"type", "getCard"},
                       {"nickname", event.nickname}};
}

void to_json(nlohmann::json& j, const IsAuctioneer& event) {
    j = nlohmann::json{{"type", "isAuctioneer"},
                       {"nickname", event.nickname}};
}

void to_json(nlohmann::json& j, const JoinMatchResult& event) {
    j = nlohmann::json{{"type", "joinMatch"},
                       {"nickname", event.nickname}};
}

void to_json(nlohmann::json& j, const Leave& event) {
    j = nlohmann::json{{"type", "leave"},
                       {"nickname", event.nickname},
                       {"reason", event.reason}};
}

void to_json(nlohmann::json& j, const MatchInfo& event) {
    j = nlohmann::json{{"type", "matchInfo"},
                       {"deck", event.deck},
                       {"players", event.players},
                       {"matchConfig", event.config}};
}

void to_json(nlohmann::json& j, const OfferResult& event) {
    j = nlohmann::json{{"type", "offer"},
                       {"nickname", event.nickname},
                       {"gold", event.gold}};
}

void to_json(nlohmann::json& j, const PassResult& event) {
    j = nlohmann::json{{"type", "pass"},
                       {"nickname", event.nickname}};
}

void to_json(nlohmann::json& j, const Pay& event) {
    j = nlohmann::json{{"type", "pay"},
                       {"nickname", event.nickname},
                       {"gold", event.gold}};
}

void to_json(nlohmann::json& j, const SetGold& event) {
    j = nlohmann::json{{"type", "setGold"},
                       {"nickname", event.nickname},
                       {"gold", event.gold}};
}

void to_json(nlohmann::json& j, const Winner& event) {
    j = nlohmann::json{{"type", "winner"},
                       {"nickname", event.nickname}};
}

void to_json(nlohmann::json& j, const OutputEvent& event) {
    std::visit(visitor{
                   [&](const std::monostate& /**/) {},
                   [&](const auto& e) { j = e; },
               },
               event);
}

[[nodiscard]] std::string toMessage(const OutputEvent& event) {
    nlohmann::json j = event;
    return j.dump();
}

}  // namespace Events
