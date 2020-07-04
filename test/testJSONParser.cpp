#include <catch2/catch.hpp>
#include <memory>
#include <string>
#include <vector>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS  //Avoid warning
#include <boost/iostreams/stream.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "Events/AttackRequest.hpp"
#include "Events/AttackResult.hpp"
#include "Events/Draw.hpp"
#include "Events/Earn.hpp"
#include "Events/Error.hpp"
#include "Events/GetCard.hpp"
#include "Events/InputEvent.hpp"
#include "Events/IsAuctioneer.hpp"
#include "Events/JoinMatchRequest.hpp"
#include "Events/JoinMatchResult.hpp"
#include "Events/Leave.hpp"
#include "Events/MatchInfo.hpp"
#include "Events/OfferRequest.hpp"
#include "Events/OfferResult.hpp"
#include "Events/OutputEvent.hpp"
#include "Events/PassRequest.hpp"
#include "Events/PassResult.hpp"
#include "Events/Pay.hpp"
#include "Events/SetGold.hpp"
#include "Events/Winner.hpp"
#include "Game/Card.hpp"
#include "Game/Deck.hpp"
#include "Game/MatchConfig.hpp"
#include "Game/Player.hpp"
#include "Parsing/JSONParser.hpp"
#include "Utils/Time.hpp"

using IT = InputEvent::Type;
using OT = OutputEvent::Type;
TEST_CASE("Test JSONParser::messageToInputEvent", "[parser]") {
    const auto now = Clock::now();

    SECTION("JoinMatchRequest") {
        constexpr auto message = R"(
            {"type": "joinMatchRequest", 
            "matchID": "Match123", 
            "nickname": "Player1"})";
        auto event = JSONParser::messageToInputEvent(now, "", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == IT::JoinMatchRequest);
        JoinMatchRequest req = *static_cast<JoinMatchRequest*>(event.get());
        REQUIRE(req.time == now);
        REQUIRE(req.matchID == "Match123");
        REQUIRE(req.nickname == "Player1");
    }

    SECTION("Invalid JoinMatchRequest (matchID)") {
        constexpr auto message = R"(
            {"type": "joinMatchRequest",
            "nickname": "Player1"})";
        auto event = JSONParser::messageToInputEvent(now, "", message);
        REQUIRE_FALSE(event->isValid());
    }

    SECTION("Invalid JoinMatchRequest (nickname)") {
        constexpr auto message = R"(
            {"type": "joinMatchRequest",
            "matchID": "Match123"})";
        auto event = JSONParser::messageToInputEvent(now, "", message);
        REQUIRE_FALSE(event->isValid());
    }

    SECTION("AttackRequest") {
        constexpr auto message = R"(
            {"type": "attack"})";
        auto event = JSONParser::messageToInputEvent(now, "Player1", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == IT::AttackRequest);
        AttackRequest att = *static_cast<AttackRequest*>(event.get());
        REQUIRE(att.time == now);
        REQUIRE(att.nickname == "Player1");
    }

    SECTION("PassRequest") {
        constexpr auto message = R"(
            {"type": "pass"})";
        auto event = JSONParser::messageToInputEvent(now, "Player1", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == IT::PassRequest);
        PassRequest pass = *static_cast<PassRequest*>(event.get());
        REQUIRE(pass.time == now);
        REQUIRE(pass.nickname == "Player1");
    }

    SECTION("OfferRequest") {
        constexpr auto message = R"(
            {"type": "offer",
            "gold": 432})";
        auto event = JSONParser::messageToInputEvent(now, "Player1", message);
        REQUIRE(event->isValid());
        REQUIRE(event->getType() == IT::OfferRequest);
        OfferRequest offer = *static_cast<OfferRequest*>(event.get());
        REQUIRE(offer.time == now);
        REQUIRE(offer.nickname == "Player1");
        REQUIRE(offer.gold == 432);
    }

    SECTION("Invalid OfferRequest") {
        constexpr auto message = R"(
            {"type": "offer",
            "gold": "ten"})";
        auto event = JSONParser::messageToInputEvent(now, "Player1", message);
        REQUIRE_FALSE(event->isValid());
    }

    SECTION("Invalid input") {
        constexpr auto message = R"(
            {"typ)";
        auto event = JSONParser::messageToInputEvent(now, "Player1", message);
        REQUIRE_FALSE(event->isValid());
    }
}

TEST_CASE("Test JSONParser::outputEventToMessage", "[parser]") {
    // String to ptree
    auto parseJSON = [](const std::string& json) {
        boost::iostreams::stream<boost::iostreams::array_source> stream(json.c_str(), json.size());
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(stream, pt);
        return pt;
    };

    // Checks equality of Card and a Card's ptree
    auto checkCardTree = [](const Card& c, const boost::property_tree::ptree& pt) {
        REQUIRE(pt.get<int>("id") == c.id);
        REQUIRE(pt.get<std::string>("name") == c.getName());
        REQUIRE(pt.get<int>("attack") == c.getAttack());
        REQUIRE(pt.get<int>("production") == c.getProduction());
        REQUIRE(pt.get<int>("victory") == c.getVictory());
        REQUIRE(pt.get<bool>("isBerserk") == c.isBerserk());
    };

    // Checks equality of Deck and a Deck's ptree
    auto checkDeckTree = [checkCardTree](const Deck& d, const boost::property_tree::ptree& pt) {
        const auto cards = d.getCards();
        const auto treeCards = pt.get_child("cards");
        REQUIRE(cards.size() == treeCards.size());
        unsigned int i = 0;
        for (const auto& c : treeCards) {
            checkCardTree(cards[i], c.second);
            ++i;
        }
    };

    // Checks equality of Player and a Player's ptree
    auto checkPlayerTree = [](const Player& p, const boost::property_tree::ptree& pt) {
        REQUIRE(pt.get<std::string>("nickname") == p.name);
    };
    auto checkPlayerVectorTree = [checkPlayerTree](const std::vector<Player>& players, const boost::property_tree::ptree& pt) {
        REQUIRE(players.size() == pt.size());
        unsigned int i = 0;
        for (const auto& p : pt) {
            checkPlayerTree(players[i], p.second);
            ++i;
        }
    };

    // Checks equality of MatchConfig and a MatchConfig's ptree
    auto checkMatchConfigTree = [](const MatchConfig& c, const boost::property_tree::ptree& pt) {
        REQUIRE(pt.get<unsigned int>("numPlayers") == c.numPlayers);
        // Not tested for now
        // REQUIRE(pt.get<int>("cardsPerTurn") == c.cardsPerTurn);
        // REQUIRE(pt.get<int>("initialGold") == c.initialGold);
        // REQUIRE(pt.get<int>("auctioneerGoldDisadvantage") == c.auctioneerGoldDisadvantage);
    };

    SECTION("JoinMatch") {
        JoinMatchResult joinMatch("Player1", "Match123");
        const auto message = JSONParser::outputEventToMessage(&joinMatch);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "joinMatch");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<std::string>("matchID") == "Match123");
    }

    SECTION("Attack") {
        AttackResult attack("Player1");
        const auto message = JSONParser::outputEventToMessage(&attack);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "attack");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("Offer") {
        OfferResult offer("Player1", 30);
        const auto message = JSONParser::outputEventToMessage(&offer);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "offer");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<int>("gold") == 30);
    }

    SECTION("Pass") {
        PassResult pass("Player1");
        const auto message = JSONParser::outputEventToMessage(&pass);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "pass");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("Error") {
        Error error("Invalid action received");
        const auto message = JSONParser::outputEventToMessage(&error);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "error");
        REQUIRE(pt.get<std::string>("message") == "Invalid action received");
    }

    SECTION("Pay") {
        Pay pay("Player1", 432);
        const auto message = JSONParser::outputEventToMessage(&pay);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "pay");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<int>("gold") == 432);
    }

    SECTION("Earn") {
        Earn earn("Player1", 432);
        const auto message = JSONParser::outputEventToMessage(&earn);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "earn");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<int>("gold") == 432);
    }

    SECTION("Winner") {
        Winner winner("Player1");
        const auto message = JSONParser::outputEventToMessage(&winner);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "winner");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("GetCard") {
        GetCard getCard("Player1");
        const auto message = JSONParser::outputEventToMessage(&getCard);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "getCard");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("IsAuctioneer") {
        IsAuctioneer isAuctioneer("Player1");
        const auto message = JSONParser::outputEventToMessage(&isAuctioneer);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "isAuctioneer");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
    }

    SECTION("Leave") {
        Leave leave("Player1", "Manually disconnected");
        const auto message = JSONParser::outputEventToMessage(&leave);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "leave");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<std::string>("reason") == "Manually disconnected");
    }

    SECTION("SetGold") {
        SetGold setGold("Player1", 10);
        const auto message = JSONParser::outputEventToMessage(&setGold);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "setGold");
        REQUIRE(pt.get<std::string>("nickname") == "Player1");
        REQUIRE(pt.get<int>("gold") == 10);
    }

    SECTION("Draw") {
        // Needed initialisation of cards
        CardModel::add(1, "Inquisidor", 1, 0, 1, false);
        Card::add(1, {}, 1);

        const auto card = Card::getById(1);
        Draw draw(card);
        const auto message = JSONParser::outputEventToMessage(&draw);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "draw");
        const auto cardTree = pt.get_child("card");
        checkCardTree(card, cardTree);
    }

    SECTION("MatchInfo") {
        // Initialisation needed to instantiate a test Card
        CardModel::add(2, "Inquisidor", 1, 0, 1, false);
        CardModel::add(3, "Campo", 2, 0, 1, false);
        Card::add(2, {}, 2);
        Card::add(3, {}, 3);
        Card::add(4, {}, 3);
        Deck::add(1, {2, 3, 4});
        const auto deck = Deck::getById(1);

        // Change default values of config
        auto config = MatchConfig(5);
        config.cardsPerTurn = 10;
        config.auctioneerGoldDisadvantage = 0;
        config.initialGold = 20;

        // Vector of players
        std::vector<Player> players;
        players.emplace_back("Alpha");
        players.emplace_back("Bravo");
        players.emplace_back("Charlie");
        players.emplace_back("Delta");
        players.emplace_back("Echo");

        MatchInfo matchinfo(config, players, deck);
        const auto message = JSONParser::outputEventToMessage(&matchinfo);
        const auto pt = parseJSON(*message);
        REQUIRE(pt.get<std::string>("type") == "matchInfo");
        // Check Deck
        const auto deckTree = pt.get_child("deck");
        checkDeckTree(deck, deckTree);
        // Check Player vector
        const auto playersTree = pt.get_child("players");
        checkPlayerVectorTree(players, playersTree);
        // Check MatchConfig
        const auto configTree = pt.get_child("matchConfig");
        checkMatchConfigTree(config, configTree);
    }
}
