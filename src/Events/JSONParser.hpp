#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>

#include "GameEvent.hpp"
#include "Time.hpp"

namespace {
using GE = GameEvent::Type;
const std::map<std::string, GE> stringToType{
    {"joinMatch", GE::JoinMatchRequest},
    {"attack", GE::Attack},
    {"offer", GE::Offer},
    {"pass", GE::Pass}};

boost::property_tree::ptree parseJSON(const std::string& json) {
    boost::iostreams::stream<boost::iostreams::array_source> stream(json.c_str(), json.size());

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::read_json(stream, pt);
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
    return pt;
}
}  // namespace

namespace JSONParser {

//TODO implement as visitor pattern (maybe not?)
//Cool C++17 way! https://www.youtube.com/watch?v=MdtYi0vvct0x
std::unique_ptr<GameEvent> messageToGameEvent(Timestamp time, const std::string& message) {
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
                if (!matchID.empty() && !nickname.empty()) {
                    return std::make_unique<JoinMatchRequest>(time, matchID, nickname);
                }
                break;
            }
            case GE::Attack:
                //TODO add cases
                break;
        }
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
    return std::make_unique<GameEvent>();
}

}  // namespace JSONParser