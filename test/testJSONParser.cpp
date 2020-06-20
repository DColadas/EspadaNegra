#include <catch2/catch.hpp>
#include <memory>
#include <string>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Events/Attack.hpp"
#include "Events/Earn.hpp"
#include "Events/GameEvent.hpp"
#include "Events/GetCard.hpp"
#include "Events/JSONParser.hpp"
#include "Events/JoinMatchRequest.hpp"
#include "Events/Offer.hpp"
#include "Events/Pass.hpp"
#include "Events/Pay.hpp"
#include "Events/Winner.hpp"
#include "Utils/Time.hpp"

TEST_CASE("Test JSONParser::messageToGameEvent", "[parser]") {
    auto now = Clock::now();

    SECTION("JoinMatchRequest") {
        constexpr auto message = R"(
            {"type": "joinMatchRequest", 
            "matchID": "Match123", 
            "nickname": "Player1"})";
        auto event = JSONParser::messageToGameEvent(now, "", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == GameEvent::Type::JoinMatchRequest);
        JoinMatchRequest req = *static_cast<JoinMatchRequest*>(event.get());
        REQUIRE(req.time == now);
        REQUIRE(req.matchID == "Match123");
        REQUIRE(req.nickname == "Player1");
    }

    SECTION("Invalid JoinMatchRequest (matchID)") {
        constexpr auto message = R"(
            {"type": "joinMatchRequest",
            "nickname": "Player1"})";
        auto event = JSONParser::messageToGameEvent(now, "", message);
        REQUIRE_FALSE(event->isValid());
    }

    SECTION("Invalid JoinMatchRequest (nickname)") {
        constexpr auto message = R"(
            {"type": "joinMatchRequest",
            "matchID": "Match123"})";
        auto event = JSONParser::messageToGameEvent(now, "", message);
        REQUIRE_FALSE(event->isValid());
    }

    SECTION("Attack") {
        constexpr auto message = R"(
            {"type": "attack"})";
        auto event = JSONParser::messageToGameEvent(now, "Player1", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == GameEvent::Type::Attack);
        Attack att = *static_cast<Attack*>(event.get());
        REQUIRE(att.time == now);
        REQUIRE(att.nickname == "Player1");
    }

    SECTION("Pass") {
        constexpr auto message = R"(
            {"type": "pass"})";
        auto event = JSONParser::messageToGameEvent(now, "Player1", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == GameEvent::Type::Pass);
        Pass pass = *static_cast<Pass*>(event.get());
        REQUIRE(pass.time == now);
        REQUIRE(pass.nickname == "Player1");
    }

    SECTION("Offer") {
        constexpr auto message = R"(
            {"type": "offer",
            "gold": 432})";
        auto event = JSONParser::messageToGameEvent(now, "Player1", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == GameEvent::Type::Offer);
        Offer offer = *static_cast<Offer*>(event.get());
        REQUIRE(offer.time == now);
        REQUIRE(offer.nickname == "Player1");
        REQUIRE(offer.gold == 432);
    }

    SECTION("Invalid input") {
        constexpr auto message = R"(
            {"typ)";
        auto event = JSONParser::messageToGameEvent(now, "Player1", message);
        REQUIRE_FALSE(event->isValid());
    }
}

TEST_CASE("Test JSONParser::gameEventToMessage", "[parser]") {
    auto parseJSON = [](const std::string& json) {
        boost::iostreams::stream<boost::iostreams::array_source> stream(json.c_str(), json.size());
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(stream, pt);
        return pt;
    };

    SECTION("Pay") {
        Pay pay("Player1", 432);
        const auto message = JSONParser::gameEventToMessage(&pay);
        REQUIRE(message);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "pay");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<int>("gold") == 432);
    }

    SECTION("Earn") {
        Earn earn("Player1", 432);
        const auto message = JSONParser::gameEventToMessage(&earn);
        REQUIRE(message);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "earn");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<int>("gold") == 432);
    }

    SECTION("Winner") {
        Winner winner("Player1");
        const auto message = JSONParser::gameEventToMessage(&winner);
        REQUIRE(message);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "winner");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("GetCard") {
        GetCard getCard("Player1");
        const auto message = JSONParser::gameEventToMessage(&getCard);
        REQUIRE(message);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "getCard");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("Invalid input") {
        GameEvent event;
        const auto message = JSONParser::gameEventToMessage(&event);
        REQUIRE_FALSE(message);
    }
}
