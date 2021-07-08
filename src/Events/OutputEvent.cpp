#include "OutputEvent.hpp"

#include <fmt/format.h>

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

void to_json(nlohmann::json& j, const Attack& event) {
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

void to_json(nlohmann::json& j, const Offer& event) {
    j = nlohmann::json{{"type", "offer"},
                       {"nickname", event.nickname},
                       {"gold", event.gold}};
}

void to_json(nlohmann::json& j, const Pass& event) {
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

void from_json(const nlohmann::json& j, OutputEvent& event) {
    enum class EventType {
        Attack_,
        Complex_,
        Draw_,
        Earn_,
        Error_,
        GetCard_,
        IsAuctioneer_,
        JoinMatchResult_,
        Leave_,
        MatchInfo_,
        Offer_,
        Pass_,
        SetGold_,
        Winner_,
        Invalid
    };
    using ET = EventType;  // Clang 12 doesn't support using enum :)

    constexpr static auto toType = [&](std::string_view text) {
        if (text == "attack") {
            return ET::Attack_;
        } else if (text == "complex") {
            return ET::Complex_;
        } else if (text == "draw") {
            return ET::Draw_;
        } else if (text == "earn") {
            return ET::Earn_;
        } else if (text == "error") {
            return ET::Error_;
        } else if (text == "getCard") {
            return ET::GetCard_;
        } else if (text == "isAuctioneer") {
            return ET::IsAuctioneer_;
        } else if (text == "joinMatch") {
            return ET::JoinMatchResult_;
        } else if (text == "leave") {
            return ET::Leave_;
        } else if (text == "matchInfo") {
            return ET::MatchInfo_;
        } else if (text == "offer") {
            return ET::Offer_;
        } else if (text == "pass") {
            return ET::Pass_;
        } else if (text == "setGold") {
            return ET::SetGold_;
        } else if (text == "winner") {
            return ET::Winner_;
        } else
            return ET::Invalid;
    };

    switch (toType(j.at("type").get<std::string_view>())) {
        case ET::Attack_:
            event = Attack{{.nickname = j.at("nickname")}};
            break;
        case ET::Complex_: {
            std::vector<OutputEvent> v;
            for (const auto& ev : j.at("events")) {
                v.push_back(toOutputEvent(ev.get<std::string_view>()));
            }
            event = Complex{std::move(v)};
        } break;
        case ET::Draw_:
            event = Draw{.card = j.at("card")};
            break;
        // case ET::Earn_:
        //     event = Earn{{.nickname = j.at("nickname")}, j.at("gold")};
        //     break;
        case ET::Error_:
            event = Error{.message = j.at("message")};
            break;
        // case ET::GetCard_:
        //     event = GetCard{{.nickname = j.at("nickname")}};
        //     break;
        // case ET::IsAuctioneer_:
        //     event = IsAuctioneer{{.nickname = j.at("nickname")}};
        //     break;
        case ET::JoinMatchResult_:
            event = JoinMatchResult{j.at("nickname")};
            break;
        case ET::Offer_:
            event = Offer{{j.at("nickname")}, j.at("gold")};
            break;
        case ET::Pass_:
            event = Pass{j.at("nickname")};
            break;
        case ET::MatchInfo_:
            event = MatchInfo{
                j.at("matchConfig"),
                j.at("players"),
                Model::Deck{j.at("deck").get<std::vector<Model::Card>>()}};
            break;
        case ET::Invalid:
            event = Error{fmt::format("{} is not a valid input type", j.at("type").get<std::string_view>())};
            break;
        default:
            // TODO
            event = {};
            break;
    }
}

[[nodiscard]] OutputEvent toOutputEvent(std::string_view message) {
    try {
        const auto j = nlohmann::json::parse(message);
        OutputEvent e = j;
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
