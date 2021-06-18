import { ClientWS } from "./ClientWS.js";
import { Match } from "./Match.js";
import { Player } from "./Player.js";
import { EventDispatcher } from "./EventDispatcher.js";
import { View } from "./View.js";

const serverURL = document.getElementById("serverURL");
const nickname = document.getElementById("nickname");
const matchID = document.getElementById("matchID");
const joinMatchButton = document.getElementById("joinMatch");
const attackButton = document.getElementById("attack");
const offeredGold = document.getElementById("offeredGold");
const offerButton = document.getElementById("offer");
const passButton = document.getElementById("pass");

let match = new Match();
let view = new View();
let dispatcher = new EventDispatcher();
let ws;
let currentNickname;


// Bind local events callbacks
joinMatchButton.addEventListener("click", () => {
    if (ws != null) {
        ws.close();
    }
    ws = new ClientWS(serverURL.value);
    ws.dispatcher = dispatcher;
    ws.joinMatch(matchID.value, nickname.value);
    currentNickname = nickname.value;
});

attackButton.addEventListener("click", () => {
    ws.attack();
});

offerButton.addEventListener("click", () => {
    ws.offer(offeredGold.value);
});

passButton.addEventListener("click", () => {
    ws.pass();
});

// Bind callbacks for incoming server events
dispatcher.onAttack = (nickname) => {
    match.attack(nickname);
};

dispatcher.onDraw = (cardID) => {
    match.draw(cardID);
    view.draw(match.deck[cardID]);
};

dispatcher.onEarn = (nickname, gold) => {
    match.earn(nickname, gold);
};

dispatcher.onGetCard = (nickname) => {
    match.getCard(nickname);
    view.getCard(nickname, null);
};

dispatcher.onIsAuctioneer = (nickname) => {
    match.setAuctioneer(nickname);
};

dispatcher.onJoinMatch = (nickname) => {
    match.addPlayer(nickname);
    view.addPlayer(nickname);
};

dispatcher.onLeave = (nickname, reason) => {
    //TODO
};

dispatcher.onMatchInfo = (config, players, deck) => {
    // Match takes a {nickname: Player} object with all the cards
    let dictPlayers = {};
    // Match takes a {cardID: Card} object with all the cards
    let dictDeck = {};
    players.forEach(p => {
        dictPlayers[p.nickname] = new Player(p.nickname);
    });
    deck.cards.forEach(c => {
        dictDeck[c.id] = c;
    });
    match = new Match(config, dictPlayers, dictDeck);

    // Adds players (removes existing ones)
    view.resetPlayers();
    for (let p of Object.values(dictPlayers)) {
        // Don't add the client to the other players list
        if (p.nickname === currentNickname) {
            continue;
        }
        view.addPlayer(p.nickname);
    }

    // Sets client player
    view.setClientPlayer(currentNickname);
};

dispatcher.onOffer = (nickname, gold) => {
    match.offer(nickname, gold);
};

dispatcher.onPass = (nickname) => {
    match.pass(nickname);
};

dispatcher.onPay = (nickname, gold) => {
    match.pay(nickname, gold);
};

dispatcher.onSetGold = (nickname, gold) => {
    match.setGold(nickname, gold);
};

dispatcher.onWinner = (nickname) => {
    //TODO
};

dispatcher.onError = (message) => {
    view.setError(message);
};

// God, I hate JavaScript
dispatcher._dispatchEvent = dispatcher.dispatchEvent;
dispatcher.dispatchEvent = (event) => {
    view.resetError();
    dispatcher._dispatchEvent(event);
};

