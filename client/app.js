import { ClientWS } from "./ClientWS.js";

// Get HTML elements
const joinMatchButton = document.getElementById("joinMatch");
const matchID = document.getElementById("matchID");
const nickname = document.getElementById("nickname");

let ws = new ClientWS();

joinMatchButton.addEventListener("click", () => {
    ws.joinMatch(matchID.value, nickname.value);
});

