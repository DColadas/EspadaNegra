import { ClientWS } from "./ClientWS.js";

// Get HTML elements
const serverURL = document.getElementById("serverURL");
const nickname = document.getElementById("nickname");
const matchID = document.getElementById("matchID");
const joinMatchButton = document.getElementById("joinMatch");
const attackButton = document.getElementById("attack");
const offeredGold = document.getElementById("offeredGold");
const offerButton = document.getElementById("offer");
const passButton = document.getElementById("pass");

let ws;

joinMatchButton.addEventListener("click", () => {
    if (ws != null) {
        ws.close();
    }
    ws = new ClientWS(serverURL.value);
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

