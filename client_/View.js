// Get HTML elements
const errorTag = document.getElementById("error");
const cardsInPlayList = document.getElementById("cards-in-play");
const playerList = document.getElementById("players");
const clientPlayer = document.getElementById("client-player");

const PLACEHOLDER_IMAGE = "https://espadanegra.net/gal.php?id=444";
const createDiv = (cls, content) => {
    let div = document.createElement("div");
    div.className = cls;
    div.innerHTML = content;
    return div;
};
export class View {
    constructor() { }

    draw(card) {
        let newCard = document.createElement("img");
        newCard.src = card.image ? card.image : PLACEHOLDER_IMAGE;
        // If it's the only card in the auction, set as current card
        if (!cardsInPlayList.getElementsByClassName("current-card").length) {
            newCard.className = "current-card";
        } else {
            newCard.className = "future-card";
        }
        cardsInPlayList.appendChild(newCard);
    }

    getCard(nickname, card) {
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
    }

    addPlayer(nickname) {
        let divPlayer = createDiv("player", "");
        divPlayer.appendChild(createDiv("nickname", nickname));
        playerList.appendChild(divPlayer);
    }

    resetPlayers() {
        while (playerList.lastChild) {
            playerList.removeChild(playerList.lastChild);
        }
    }

    setClientPlayer(nickname) {
        while (clientPlayer.lastChild) {
            clientPlayer.removeChild(clientPlayer.lastChild);
        }
        clientPlayer.appendChild(createDiv("nickname", nickname));
    }

    setPlayerProperties(player) {

    }

    setError(message) {
        errorTag.innerHTML = message;
    }

    resetError() {
        this.setError("");
    }

}