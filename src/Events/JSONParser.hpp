#pragma once

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <string>

#include "GameEvent.hpp"
#include "Time.hpp"

namespace {
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
    auto pt = parseJSON(message);
    try {
        const auto type = pt.get<std::string>("type");
        //TODO map type to Type enum with static function
        if (type == "joinMatch") {
            const auto matchID = pt.get<std::string>("matchID");
            const auto nickname = pt.get<std::string>("nickname");  //TODO change nickname for id of the handler
            if (!matchID.empty() && !nickname.empty()) {
                return std::make_unique<JoinMatchRequest>(time, matchID, nickname);
            }
        }
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
    return std::make_unique<GameEvent>();
}

}  // namespace JSONParser
