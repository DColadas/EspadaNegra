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
const cardsInPlayList = document.getElementById("cards-in-play");
const playerList = document.getElementById("players");
const clientPlayer = document.getElementById("client-player");

let match = new Match();
let dispatcher = new EventDispatcher();
let ws;
let currentNickname;

const PLACEHOLDER_IMAGE = "https://espadanegra.net/gal.php?id=444";

const createDiv = (cls, content) => {
    let div = document.createElement("div");
    div.className = cls;
    div.innerHTML = content;
    return div;
};

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
    let newCard = document.createElement("img");
    newCard.src = match.deck[cardID].image;
    // If it's the only card in the auction, set as current card
    if (!cardsInPlayList.getElementsByClassName("current-card").length) {
        newCard.className = "current-card";
    } else {
        newCard.className = "future-card";
    }
    cardsInPlayList.appendChild(newCard);
};

dispatcher.onEarn = (nickname, gold) => {
    match.earn(nickname, gold);
};

dispatcher.onGetCard = (nickname) => {
    match.getCard(nickname);
    cardsInPlayList.getElementsByClassName("current-card")[0].className = "removed-card";
    // If there are more cards, set the next one as current
    let futures = cardsInPlayList.getElementsByClassName("future-card");
    if (futures.length > 0) {
        futures[0].className = "current-card";
    } else {
        // No more cards left: delete everything from auction
        while (cardsInPlayList.lastChild) {
            cardsInPlayList.removeChild(cardsInPlayList.lastChild);
        }
    }
};

dispatcher.onIsAuctioneer = (nickname) => {
    match.setAuctioneer(nickname);
};

dispatcher.onJoinMatch = (nickname) => {
    match.addPlayer(nickname);

    let divPlayer = createDiv("player", "");
    divPlayer.appendChild(createDiv("nickname", nickname));
    playerList.appendChild(divPlayer);
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
        c.image = PLACEHOLDER_IMAGE;
        dictDeck[c.id] = c;
    });
    match = new Match(config, dictPlayers, dictDeck);

    // Adds players (removes existing ones)
    while (playerList.lastChild) {
        playerList.removeChild(playerList.lastChild);
    }
    for (let p of Object.values(dictPlayers)) {
        // Don't add the client to the other players list
        if (p.nickname === currentNickname) {
            continue;
        }
        let divPlayer = createDiv("player", "");
        divPlayer.appendChild(createDiv("nickname", p.nickname));
        playerList.appendChild(divPlayer);
    }

    // Sets client player
    while (clientPlayer.lastChild) {
        clientPlayer.removeChild(clientPlayer.lastChild);
    }
    clientPlayer.appendChild(createDiv("nickname", currentNickname));
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

// God, I hate JavaScript
dispatcher._dispatchEvent = dispatcher.dispatchEvent;
dispatcher.dispatchEvent = (event) => {
    error.innerHTML = "";
    dispatcher._dispatchEvent(event);
};

