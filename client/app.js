import { ClientWS } from "./ClientWS.js";

// Get HTML elements
const joinMatchButton = document.getElementById("joinMatch");
const matchID = document.getElementById("matchID");
const nickname = document.getElementById("nickname");

let ws;

joinMatchButton.addEventListener("click", () => {
    if (ws != null) {
        ws.close();
    }
    ws = new ClientWS();
    ws.joinMatch(matchID.value, nickname.value);
});

