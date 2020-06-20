const URI = "ws://localhost:8080";

export class ClientWS {

    constructor() {
        this.ws = new WebSocket(URI);
        this.ws.onopen = (ev) => {
            console.log("OPEN");
        };
        this.ws.onmessage = (ev) => {
            console.log(ev);
        };
    }

    joinMatch(matchID, nickname) {
        this.ws.send(`{"type": "joinMatchRequest", 
        "matchID": "${matchID}", 
        "nickname": "${nickname}"}`);
    }




}