#include "JSONParser.hpp"

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "Attack.hpp"
#include "Earn.hpp"
#include "GameEvent.hpp"
#include "GetCard.hpp"
#include "JoinMatchRequest.hpp"
#include "Offer.hpp"
#include "Pass.hpp"
#include "Pay.hpp"
#include "Time.hpp"
#include "Winner.hpp"

namespace {
using GE = GameEvent::Type;
const std::map<std::string, GE> stringToType{
    {"joinMatchRequest", GE::JoinMatchRequest},
    {"attack", GE::Attack},
    {"offer", GE::Offer},
    {"pass", GE::Pass},
};

const std::map<GE, std::string> typeToString{
    {GE::Pay, "pay"},
    {GE::Earn, "earn"},
    {GE::Winner, "winner"},
    {GE::GetCard, "getCard"},
};

boost::property_tree::ptree parseJSON(const std::string& json) {
    boost::iostreams::stream<boost::iostreams::array_source> stream(json.c_str(), json.size());

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_json(stream, pt);
    } catch (const boost::property_tree::json_parser_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return pt;
}
}  // namespace

//TODO implement as visitor pattern (maybe not?)
//Cool C++17 way! https://www.youtube.com/watch?v=MdtYi0vvct0x
std::unique_ptr<GameEvent> JSONParser::messageToGameEvent(Timestamp time, const std::string& nickname, const std::string& message) {
    using GE = GameEvent::Type;
    auto pt = parseJSON(message);
    try {
        const auto typeString = pt.get<std::string>("type");
        const auto typeIter = stringToType.find(typeString);
        // If the received type does not exist, set type as invalid
        const auto type = (typeIter != stringToType.end()) ? typeIter->second : GE::Invalid;

        switch (type) {
            case GE::JoinMatchRequest: {
                const auto matchID = pt.get<std::string>("matchID");
                const auto nickname = pt.get<std::string>("nickname");  //TODO change nickname for id of the handler

                //if (!matchID.empty() && !nickname.empty()) {
                return std::make_unique<JoinMatchRequest>(time, matchID, nickname);
                //}
            } break;
            case GE::Attack:
                return std::make_unique<Attack>(time, nickname);
                break;
            case GE::Pass:
                return std::make_unique<Pass>(time, nickname);
                break;
            case GE::Offer: {
                const auto gold = pt.get<int>("gold");
                return std::make_unique<Offer>(time, nickname, gold);
            } break;
            case GE::Invalid:
                //TODO logger
                std::cerr << "Invalid message: " << message << std::endl;
                break;
            default:
                //TODO panic
                std::cerr << "Invalid message: " << message << std::endl;
        }
    } catch (const boost::property_tree::ptree_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return std::make_unique<GameEvent>();
}

std::unique_ptr<const std::string> JSONParser::gameEventToMessage(const GameEvent* event) {
    using GE = GameEvent::Type;
    if (!event->isValid()) {
        return nullptr;
    }
    const auto typeIter = typeToString.find(event->getType());
    // If the received type does not exist, set type as invalid
    const auto type = (typeIter != typeToString.end()) ? typeIter->second : "";

    // Add each property to a tree node
    boost::property_tree::ptree pt;
    try {
        pt.put("type", type);
        switch (event->getType()) {
            case GE::GetCard: {
                const auto getCard = static_cast<const GetCard*>(event);
                pt.put("nickname", getCard->nickname);
            } break;
            case GE::Winner: {
                const auto winner = static_cast<const Winner*>(event);
                pt.put("nickname", winner->nickname);
            } break;
            case GE::Pay: {
                const auto pay = static_cast<const Pay*>(event);
                pt.put("nickname", pay->nickname);
                pt.put("gold", pay->gold);
            } break;
            case GE::Earn: {
                const auto earn = static_cast<const Earn*>(event);
                pt.put("nickname", earn->nickname);
                pt.put("gold", earn->gold);
            } break;
            default:
                std::cerr << "Invalid event!" << std::endl;
                return nullptr;
        }
    } catch (const boost::property_tree::ptree_bad_data& e) {
        std::cerr << "put: invalid data: " << e.what() << std::endl;
    }

    // Once the tree is created, convert to JSON string
    std::stringstream ss;
    try {
        boost::property_tree::json_parser::write_json(ss, pt);
    } catch (const boost::property_tree::json_parser_error& e) {
        std::cerr << "write_json: " << e.what() << std::endl;
    }
    return std::make_unique<const std::string>(ss.str());
}
