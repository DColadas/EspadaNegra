#include <emscripten/bind.h>

#include <nlohmann/json.hpp>
#include <string>

#include "Events/OutputEvent.hpp"
#include "Game/MatchHandler.hpp"
#include "Manager/NoopManager.hpp"
#include "Model/Card.hpp"
#include "Model/Player.hpp"
#include "Utils/Visitor.hpp"

// Must be run before any other function
void init() {
    Model::Card::init();
}

std::string dispatch(const Events::OutputEvent& ev) {
    return std::visit(visitor{
                          [&](const Events::Complex&) {
                              return "complex!";
                          },
                          [&](const Events::JoinMatchResult&) {
                              return "joinMatch!";
                          },
                          [&](const Events::Attack&) {
                              return "attack!";
                          },
                          [&](const auto&) {
                              return "Other!";
                          },
                      },
                      ev);
};

Events::OutputEvent toOutputEventWrapper(const std::string& s) {
    return Events::toOutputEvent(s);
}

static bool addPlayerAndStart(MatchHandler& match, const std::string& nickname) {
    match.addPlayer(nickname);
    if (match.isFull()) {
        match.start();
    }
    return match.isRunning();
}

MatchHandler newMatch(const Model::MatchConfig& config,
                      const std::vector<Model::Player> players,
                      const Model::Deck& deck) {
    // TODO memory management,
    auto mh = MatchHandler(new NoopManager{}, config, deck);
    for (const auto& p : players) {
        addPlayerAndStart(mh, p.name);
    }

    return mh;
}

MatchHandler newMatchWrapper(const std::string& matchConfig_,
                             const std::string& players_,
                             const std::string& deck_) {
    Model::MatchConfig config = nlohmann::json::parse(matchConfig_);
    std::vector<Model::Player> players = nlohmann::json::parse(players_);
    Model::Deck deck = nlohmann::json::parse(deck_);
    return newMatch(config, players, deck);
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(events) {
    function("init", &init);
    function("toEvent", &toOutputEventWrapper);
    function("newMatch", &newMatchWrapper);
    function("dispatch", &dispatch);
    class_<Events::OutputEvent>("Event")
        .constructor<>();
    class_<MatchHandler>("Match")
        .function("isRunning", &MatchHandler::isRunning);
}