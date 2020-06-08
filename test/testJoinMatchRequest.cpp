#include <catch2/catch.hpp>
#include <memory>

#include "Events/JoinMatchRequest.hpp"

TEST_CASE("Test JoinMatchRequest correctness and slicing prevention", "[events]") {
    JoinMatchRequest j1 = JoinMatchRequest("Match1", "P1");
    std::unique_ptr<GameEvent> j2 = std::make_unique<JoinMatchRequest>(j1);
    std::unique_ptr<GameEvent> j3 = std::make_unique<GameEvent>();

    SECTION("operator== and !=") {
        auto copy = JoinMatchRequest(j1);
        REQUIRE(j1 == j1);
        REQUIRE(j1 == copy);
        REQUIRE_FALSE(j1 != j1);
        REQUIRE_FALSE(j1 != copy);
        copy.matchID = "hjfdass";
        REQUIRE_FALSE(j1 == copy);
    }

    SECTION("Copy value") {
        auto copy = JoinMatchRequest(j1);
        REQUIRE(j1 == copy);
    }

    SECTION("Copy assign value") {
        auto copy = j1;
        REQUIRE(j1 == copy);
    }

    SECTION("Move value") {
        auto copy = JoinMatchRequest(j1);
        auto moved = JoinMatchRequest(std::move(j1));
        REQUIRE(moved == copy);
    }

    SECTION("Move assign value") {
        auto copy = j1;
        auto moved = std::move(j1);
        REQUIRE(moved == copy);
    }

    SECTION("Dereference with static_cast") {
        JoinMatchRequest value = *static_cast<JoinMatchRequest*>(j2.get());
        REQUIRE(value == j1);
    }

    SECTION("Compare equal base classes") {
        JoinMatchRequest value;
        value.time = j3->time;
        // valid and type are read-only, cannot modify anything else
        REQUIRE_FALSE(value == *j3);

    }

}
