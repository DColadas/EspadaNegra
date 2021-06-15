#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>

#include "Events/InputEvent.hpp"

TEST_CASE("Test deserialization of InputEvent", "[events]") {
    SECTION("JoinMatchRequest") {
        const auto j = R"({"type": "joinMatchRequest", "matchID": "m1", "nickname": "n1"})"_json;
        const Events::InputEvent event = j;
        try {
            const auto jmr = std::get<Events::JoinMatchRequest>(event);
            REQUIRE(jmr.matchID == "m1");
            REQUIRE(jmr.nickname == "n1");
        } catch (const std::bad_variant_access& ex) {
            FAIL("JoinMatchRequest parsed incorrectly");
        }
    }

    SECTION("OfferRequest") {
        const auto j = R"({"type": "offer", "gold": 123})"_json;
        const Events::InputEvent event = j;
        try {
            const auto off = std::get<Events::OfferRequest>(event);
            REQUIRE(off.gold == 123);
        } catch (const std::bad_variant_access& ex) {
            FAIL("OfferRequest parsed incorrectly");
        }
    }

    SECTION("PassRequest") {
        const auto j = R"({"type": "pass"})"_json;
        const Events::InputEvent event = j;
        try {
            const auto pass = std::get<Events::PassRequest>(event);
        } catch (const std::bad_variant_access& ex) {
            FAIL("PassRequest parsed incorrectly");
        }
    }

    SECTION("AttackRequest") {
        const auto j = R"({"type": "attack"})"_json;
        const Events::InputEvent event = j;
        try {
            const auto att = std::get<Events::AttackRequest>(event);
        } catch (const std::bad_variant_access& ex) {
            FAIL("AttackRequest parsed incorrectly");
        }
    }

    SECTION("Invalid") {
        const auto j = R"({"type": "doesntExist", "nickname": "n1"})"_json;
        const Events::InputEvent event = j;
        try {
            std::get<std::monostate>(event);
        } catch (const std::bad_variant_access& ex) {
            FAIL("Invalid event not parsed to std::monostate");
        }
    }

    SECTION("Missing key") {
        const auto j = R"({"type": "offer", "noknem": "n1"})"_json;
        try {
            const Events::InputEvent event = j;
        } catch (const nlohmann::json::out_of_range& ex) {
            SUCCEED("Invalid event threw");
        }
    }
}
