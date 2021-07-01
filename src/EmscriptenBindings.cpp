#include <emscripten/bind.h>

#include <string>

#include "Events/OutputEvent.hpp"
#include "Utils/Visitor.hpp"

std::string dispatch(const Events::OutputEvent& ev) {
    return std::visit(visitor{
        [&](const Events::Complex&){
            return "complex!";
        },
        [&](const Events::JoinMatchResult&){
            return "joinMatch!";
        },
        [&](const Events::Attack&){
            return "attack!";
        },
        [&](const auto&) {
            return "Other!";
        },
    }, ev);
};

Events::OutputEvent toOutputEventWrapper(const std::string& s) {
    return Events::toOutputEvent(s);
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(events) {
    function("toEvent", &toOutputEventWrapper);
    function("dispatch", &dispatch);
    class_<Events::OutputEvent>("Event")
        .constructor<>();
}