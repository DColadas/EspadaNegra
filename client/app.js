import { ClientWS } from "./ClientWS.js";
import { Match } from "./Match.js";
import { Player } from "./Player.js";
import { EventDispatcher } from "./EventDispatcher.js";

// Get HTML elements
const serverURL = document.getElementById("serverURL");
const nickname = document.getElementById("nickname");
const matchID = document.getElementById("matchID");
const joinMatchButton = document.getElementById("joinMatch");
const attackButton = document.getElementById("attack");
const offeredGold = document.getElementById("offeredGold");
const offerButton = document.getElementById("offer");
const passButton = document.getElementById("pass");
const errorTag = document.getElementById("error");

let match = new Match()
let dispatcher = new EventDispatcher();
let ws;

// Bind local events callbacks
joinMatchButton.addEventListener("click", () => {
    if (ws != null) {
        ws.close();
    }
    ws = new ClientWS(serverURL.value);
    ws.dispatcher = dispatcher;
    ws.joinMatch(matchID.value, nickname.value);
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
};

dispatcher.onEarn = (nickname, gold) => {
    match.earn(nickname, gold);
};

dispatcher.onGetCard = (nickname) => {
    match.getCard(nickname);
};

dispatcher.onIsAuctioneer = (nickname) => {
    match.setAuctioneer(nickname);
};

dispatcher.onJoinMatch = (nickname) => {
    match.addPlayer(nickname);
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
    error.innerHTML = message;
};

