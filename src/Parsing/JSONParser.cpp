#include "JSONParser.hpp"

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <map>
#include <memory>
#include <string>

#include "Events/AttackRequest.hpp"
#include "Events/AttackResult.hpp"
#include "Events/Complex.hpp"
#include "Events/Draw.hpp"
#include "Events/Earn.hpp"
#include "Events/Error.hpp"
#include "Events/GetCard.hpp"
#include "Events/InputEvent.hpp"
#include "Events/IsAuctioneer.hpp"
#include "Events/JoinMatchRequest.hpp"
#include "Events/JoinMatchResult.hpp"
#include "Events/Leave.hpp"
#include "Events/MatchInfo.hpp"
#include "Events/OfferRequest.hpp"
#include "Events/OfferResult.hpp"
#include "Events/OutputEvent.hpp"
#include "Events/PassRequest.hpp"
#include "Events/PassResult.hpp"
#include "Events/Pay.hpp"
#include "Events/SetGold.hpp"
#include "Events/Winner.hpp"
#include "Game/Card.hpp"
#include "Game/Deck.hpp"
#include "Game/MatchConfig.hpp"
#include "Game/Player.hpp"
#include "Logging/Logger.hpp"
#include "Utils/Time.hpp"

using IT = InputEvent::Type;
using OT = OutputEvent::Type;
namespace {
const std::map<std::string, IT> stringToType{
    {"joinMatchRequest", IT::JoinMatchRequest},
    {"attack", IT::AttackRequest},
    {"offer", IT::OfferRequest},
    {"pass", IT::PassRequest},
};

const std::map<OT, std::string> typeToString{
    {OT::JoinMatchResult, "joinMatch"},
    {OT::AttackResult, "attack"},
    {OT::OfferResult, "offer"},
    {OT::PassResult, "pass"},
    {OT::Error, "error"},
    {OT::Pay, "pay"},
    {OT::Draw, "draw"},
    {OT::Earn, "earn"},
    {OT::Winner, "winner"},
    {OT::GetCard, "getCard"},
    {OT::IsAuctioneer, "isAuctioneer"},
    {OT::Leave, "leave"},
    {OT::MatchInfo, "matchInfo"},
    {OT::SetGold, "setGold"},
    {OT::Complex, "complex"},
};

boost::property_tree::ptree parseJSON(const std::string& json) {
    boost::iostreams::stream<boost::iostreams::array_source> stream(json.c_str(), json.size());

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_json(stream, pt);
    } catch (const boost::property_tree::json_parser_error& e) {
        LOG_DEBUG(e.what());
    }
    return pt;
}

std::string treeToString(const boost::property_tree::ptree& pt) {
    std::stringstream ss;
    try {
        boost::property_tree::json_parser::write_json(ss, pt);
    } catch (const boost::property_tree::json_parser_error& e) {
        LOG_ERROR(e.what());
    }
    return ss.str();
}

boost::property_tree::ptree cardToTree(const Card& card) {
    boost::property_tree::ptree pt;
    try {
        pt.put("id", card.id);
        pt.put("name", card.getName());
        pt.put("attack", card.getAttack());
        pt.put("production", card.getProduction());
        pt.put("victory", card.getVictory());
        pt.put("isBerserk", card.isBerserk());
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }
    return pt;
}

boost::property_tree::ptree deckToTree(const Deck& deck) {
    boost::property_tree::ptree pt, children;
    const auto& cards = deck.getCards();
    try {
        for (const auto& c : cards) {
            const auto& parsedCard = cardToTree(c);
            children.push_back(std::make_pair("", parsedCard));
        }
        pt.add_child("cards", children);
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }
    return pt;
}

boost::property_tree::ptree playerToTree(const Player& player) {
    boost::property_tree::ptree pt;
    try {
        pt.put("nickname", player.name);
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }
    return pt;
}

// Returns array of players without name
boost::property_tree::ptree playerVectorToTree(const std::vector<Player>& players) {
    boost::property_tree::ptree children;
    try {
        for (const auto& p : players) {
            const auto& parsedPlayer = playerToTree(p);
            children.push_back(std::make_pair("", parsedPlayer));
        }
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }
    return children;
}

boost::property_tree::ptree matchConfigToTree(const MatchConfig& config) {
    boost::property_tree::ptree pt;
    try {
        pt.put("numPlayers", config.numPlayers);
        //TODO this is a placeholder. For now the config is not used at all
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }
    return pt;
}

boost::property_tree::ptree outputEventToTree(const OutputEvent* event) {
    const auto typeIter = typeToString.find(event->getType());
    // If the received type does not exist, set type as invalid
    const auto type = (typeIter != typeToString.end()) ? typeIter->second : "";

    // Add each property to a tree node
    boost::property_tree::ptree pt;
    try {
        pt.put("type", type);
        switch (event->getType()) {
            case OT::JoinMatchResult: {
                const auto joinMatchResult = static_cast<const JoinMatchResult*>(event);
                pt.put("nickname", joinMatchResult->nickname);
                pt.put("matchID", joinMatchResult->matchID);
            } break;
            case OT::AttackResult: {
                const auto attackResult = static_cast<const AttackResult*>(event);
                pt.put("nickname", attackResult->nickname);
            } break;
            case OT::OfferResult: {
                const auto offerResult = static_cast<const OfferResult*>(event);
                pt.put("nickname", offerResult->nickname);
                pt.put("gold", offerResult->gold);
            } break;
            case OT::PassResult: {
                const auto passResult = static_cast<const PassResult*>(event);
                pt.put("nickname", passResult->nickname);
            } break;
            case OT::GetCard: {
                const auto getCard = static_cast<const GetCard*>(event);
                pt.put("nickname", getCard->nickname);
            } break;
            case OT::Winner: {
                const auto winner = static_cast<const Winner*>(event);
                pt.put("nickname", winner->nickname);
            } break;
            case OT::Pay: {
                const auto pay = static_cast<const Pay*>(event);
                pt.put("nickname", pay->nickname);
                pt.put("gold", pay->gold);
            } break;
            case OT::Earn: {
                const auto earn = static_cast<const Earn*>(event);
                pt.put("nickname", earn->nickname);
                pt.put("gold", earn->gold);
            } break;
            case OT::Error: {
                const auto error = static_cast<const Error*>(event);
                pt.put("message", error->message);
            } break;
            case OT::IsAuctioneer: {
                const auto isAuctioneer = static_cast<const IsAuctioneer*>(event);
                pt.put("nickname", isAuctioneer->nickname);
            } break;
            case OT::Leave: {
                const auto leave = static_cast<const Leave*>(event);
                pt.put("nickname", leave->nickname);
                pt.put("reason", leave->reason);
            } break;
            case OT::SetGold: {
                const auto setGold = static_cast<const SetGold*>(event);
                pt.put("nickname", setGold->nickname);
                pt.put("gold", setGold->gold);
            } break;
            case OT::MatchInfo: {
                const auto matchInfo = static_cast<const MatchInfo*>(event);
                pt.add_child("deck", deckToTree(matchInfo->deck));
                pt.add_child("players", playerVectorToTree(matchInfo->players));
                pt.add_child("matchConfig", matchConfigToTree(matchInfo->config));
            } break;
            case OT::Draw: {
                const auto draw = static_cast<const Draw*>(event);
                pt.add_child("card", cardToTree(draw->card));
            } break;
            case OT::Complex: {
                const auto complex = static_cast<const Complex*>(event);
                const auto& events = complex->getEvents();
                boost::property_tree::ptree children;
                // Parse every event into its own tree, add to array
                for (const auto& e : events) {
                    children.push_back({"", outputEventToTree(e.get())});
                }
                pt.add_child("events", children);
            } break;
            default:
                LOG_PANIC("Not implemented OutputEvent");
                return {};
        }
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }
    return pt;
}

}  // namespace

//TODO implement as visitor pattern (maybe not?)
//Cool C++17 way! https://www.youtube.com/watch?v=MdtYi0vvct0x
std::unique_ptr<InputEvent> JSONParser::messageToInputEvent(Timestamp time,
                                                            const std::string& nickname,
                                                            const std::string& message) {
    auto pt = parseJSON(message);
    try {
        const auto typeString = pt.get<std::string>("type");
        const auto typeIter = stringToType.find(typeString);
        // If the received type does not exist, set type as invalid
        const auto type = (typeIter != stringToType.end()) ? typeIter->second : IT::Invalid;

        switch (type) {
            case IT::JoinMatchRequest: {
                const auto matchID = pt.get<std::string>("matchID");
                const auto jsonNickname = pt.get<std::string>("nickname");  //TODO change nickname for id of the handler

                //if (!matchID.empty() && !nickname.empty()) {
                return std::make_unique<JoinMatchRequest>(time, jsonNickname, matchID);
                //}
            } break;
            case IT::AttackRequest:
                return std::make_unique<AttackRequest>(time, nickname);
                break;
            case IT::PassRequest:
                return std::make_unique<PassRequest>(time, nickname);
                break;
            case IT::OfferRequest: {
                const auto gold = pt.get<int>("gold");
                return std::make_unique<OfferRequest>(time, nickname, gold);
            } break;
            case IT::Invalid:
                LOG_DEBUG("Invalid message: " + message);
                break;
            default:
                LOG_PANIC("Not implemented: " + message);
        }
    } catch (const boost::property_tree::ptree_error& e) {
        LOG_DEBUG(e.what());
    }
    return std::make_unique<InputEvent>();
}

std::unique_ptr<const std::string> JSONParser::outputEventToMessage(const OutputEvent* event) {
    LOG_PANIC_IF(!event, "nullptr received");
    const auto pt = outputEventToTree(event);

    // Once the tree is created, convert to JSON string
    auto str = treeToString(pt);
    return std::make_unique<const std::string>(std::move(str));
}
