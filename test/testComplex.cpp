#include <catch2/catch.hpp>
#include <memory>

#include "Events/AttackResult.hpp"
#include "Events/Complex.hpp"
#include "Events/GetCard.hpp"
#include "Events/OutputEvent.hpp"

using OT = OutputEvent::Type;
TEST_CASE("Test creation of Complex", "[events]") {
    SECTION("Complex::add()") {
        Complex c;
        c.add(std::make_unique<AttackResult>("Player1"));
        c.add(std::make_unique<GetCard>("Player2"));
        const auto& events = c.getEvents();
        const auto& event0 = static_cast<AttackResult*>(events[0].get());
        const auto& event1 = static_cast<GetCard*>(events[1].get());
        REQUIRE(c.getType() == OT::Complex);
        REQUIRE(event0->getType() == OT::AttackResult);
        REQUIRE(event0->nickname == "Player1");
        REQUIRE(event1->getType() == OT::GetCard);
        REQUIRE(event1->nickname == "Player2");
    }
}
