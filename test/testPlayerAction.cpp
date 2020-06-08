#include <catch2/catch.hpp>
#include <memory>

#include "Events/Attack.hpp"
#include "Events/Offer.hpp"
#include "Events/Pass.hpp"
#include "Events/PlayerAction.hpp"

TEST_CASE("Test error prevention between PlayerAction hierarchy", "[events]") {
    Attack a1 = Attack("P1");
    Pass p1 = Pass("P1");
    Offer o1 = Offer("P1", 10);
    std::unique_ptr<PlayerAction> a2 = std::make_unique<Attack>(a1);
    //std::unique_ptr<GameEvent> j3 = std::make_unique<GameEvent>();

    SECTION("operator== and !=") {
        auto copy = Attack(a1);
        REQUIRE(a1 == a1);
        REQUIRE(a1 == copy);
        REQUIRE_FALSE(a1 != a1);
        REQUIRE_FALSE(a1 != copy);
        copy.nickname = "hjfdass";
        REQUIRE_FALSE(a1 == copy);
    }

    SECTION("Copy value") {
        auto copy = Attack(a1);
        REQUIRE(a1 == copy);
    }

    SECTION("Copy assign value") {
        auto copy = a1;
        REQUIRE(a1 == copy);
    }

    SECTION("Move value") {
        auto copy = Attack(a1);
        auto moved = Attack(std::move(a1));
        REQUIRE(moved == copy);
    }

    SECTION("Move assign value") {
        auto copy = a1;
        auto moved = std::move(a1);
        REQUIRE(moved == copy);
    }

    SECTION("Dereference with static_cast") {
        Attack value = *static_cast<Attack*>(a2.get());
        REQUIRE(value == a1);
    }

    SECTION("Compare equal base classes") {
        // Only difference between a1 and p1 is the type 
        a1.time = p1.time;
        REQUIRE_FALSE(a1 == p1);
    }
}
