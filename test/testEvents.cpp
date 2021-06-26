#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>

#include "Events/InputEvent.hpp"
#include "Events/OutputEvent.hpp"

TEST_CASE("Test deserialization of InputEvent", "[events]") {
    SECTION("JoinMatchRequest") {
        const Events::InputEvent event = Events::toInputEvent(
            {}, R"({"type": "joinMatchRequest", "matchID": "m1", "nickname": "n1"})");
        try {
            const auto jmr = std::get<Events::JoinMatchRequest>(event);
            REQUIRE(jmr.matchID == "m1");
            REQUIRE(jmr.nickname == "n1");
        } catch (const std::bad_variant_access& ex) {
            FAIL("JoinMatchRequest parsed incorrectly");
        }
    }

    SECTION("OfferRequest") {
        const Events::InputEvent event = Events::toInputEvent(
            "n1", R"({"type": "offer", "gold": 123})");
        try {
            const auto off = std::get<Events::OfferRequest>(event);
            REQUIRE(off.nickname == "n1");
            REQUIRE(off.gold == 123);
        } catch (const std::bad_variant_access& ex) {
            FAIL("OfferRequest parsed incorrectly");
        }
    }

    SECTION("PassRequest") {
        const Events::InputEvent event = Events::toInputEvent(
            "n1", R"({"type": "pass"})");
        try {
            const auto pass = std::get<Events::PassRequest>(event);
            REQUIRE(pass.nickname == "n1");
        } catch (const std::bad_variant_access& ex) {
            FAIL("PassRequest parsed incorrectly");
        }
    }

    SECTION("AttackRequest") {
        const Events::InputEvent event = Events::toInputEvent(
            "n1", R"({"type": "attack"})");
        try {
            const auto att = std::get<Events::AttackRequest>(event);
            REQUIRE(att.nickname == "n1");
        } catch (const std::bad_variant_access& ex) {
            FAIL("AttackRequest parsed incorrectly");
        }
    }

    SECTION("Invalid") {
        const Events::InputEvent event = Events::toInputEvent(
            "n2", R"({"type": "doesntExist", "nickname": "n1"})");
        try {
            std::get<Events::Error>(event);
        } catch (const std::bad_variant_access& ex) {
            FAIL("Invalid event not parsed to Error");
        }
    }

    SECTION("Missing key") {
        const Events::InputEvent event = Events::toInputEvent(
            "n2", R"({"type": "offer", "noknem": "n1"})");
        try {
            std::get<Events::Error>(event);
        } catch (const nlohmann::json::out_of_range& ex) {
            FAIL("Invalid event not parsed to Error");
        }
    }
}

TEST_CASE("Test serialization of OutputEvent", "[events]") {
    SECTION("Attack") {
        const Events::OutputEvent e = Events::Attack{{"n1"}};
        const nlohmann::json j = e;
        REQUIRE(j.at("type") == "attack");
        REQUIRE(j.at("nickname") == "n1");
    }

    SECTION("Complex") {
        const Events::OutputEvent e = Events::Complex{
            {Events::Attack{"n1"},
             Events::Earn{{"n2"}, 123}}};
        const nlohmann::json j = e;
        const auto events = j.at("events");
        const auto& e0 = events[0];
        const auto& e1 = events[1];

        REQUIRE(j.at("type") == "complex");
        REQUIRE(e0.at("type") == "attack");
        REQUIRE(e0.at("nickname") == "n1");
        REQUIRE(e1.at("type") == "earn");
        REQUIRE(e1.at("nickname") == "n2");
        REQUIRE(e1.at("gold") == 123);
    }

    SECTION("Earn") {
        const Events::OutputEvent e = Events::Earn{{"n1"}, 123};
        const nlohmann::json j = e;
        REQUIRE(j.at("type") == "earn");
        REQUIRE(j.at("nickname") == "n1");
        REQUIRE(j.at("gold") == 123);
    }
}
