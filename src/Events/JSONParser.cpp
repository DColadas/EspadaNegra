#include "JSONParser.hpp"

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <map>
#include <memory>
#include <string>

#include "AttackRequest.hpp"
#include "Earn.hpp"
#include "GetCard.hpp"
#include "InputEvent.hpp"
#include "JoinMatchRequest.hpp"
#include "Logging/Logger.hpp"
#include "OfferRequest.hpp"
#include "OutputEvent.hpp"
#include "PassRequest.hpp"
#include "Pay.hpp"
#include "Utils/Time.hpp"
#include "Winner.hpp"

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
    {OT::Pay, "pay"},
    {OT::Earn, "earn"},
    {OT::Winner, "winner"},
    {OT::GetCard, "getCard"},
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
    const auto typeIter = typeToString.find(event->getType());
    // If the received type does not exist, set type as invalid
    const auto type = (typeIter != typeToString.end()) ? typeIter->second : "";

    // Add each property to a tree node
    boost::property_tree::ptree pt;
    try {
        pt.put("type", type);
        switch (event->getType()) {
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
            default:
                LOG_PANIC("Not implemented OutputEvent");
                return nullptr;
        }
    } catch (const boost::property_tree::ptree_bad_data& e) {
        LOG_ERROR(e.what());
    }

    // Once the tree is created, convert to JSON string
    std::stringstream ss;
    try {
        boost::property_tree::json_parser::write_json(ss, pt);
    } catch (const boost::property_tree::json_parser_error& e) {
        LOG_ERROR(e.what());
    }
    return std::make_unique<const std::string>(ss.str());
}
