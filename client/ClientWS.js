const URI = "ws://localhost:8080";

const sleep = async (time) => {
    return new Promise(r => setTimeout(r, time));
};

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

    async joinMatch(matchID, nickname) {
        while (this.ws.readyState === 0) {
            await sleep(100);
        }
        this.ws.send(`{"type": "joinMatchRequest", 
        "matchID": "${matchID}", 
        "nickname": "${nickname}"}`);
    }

    close() {
        this.ws.close();
    }
}