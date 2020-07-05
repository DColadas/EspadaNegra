const wsPrefix = "ws://";
const defaultURL = "localhost:8080";

const sleep = async (time) => {
    return new Promise(r => setTimeout(r, time));
};

export class ClientWS {

    constructor(serverURL) {
        if (!serverURL) {
            serverURL = defaultURL;
        }
        serverURL = wsPrefix + serverURL;
        this.ws = new WebSocket(serverURL);
        this.ws.onopen = (ev) => {
            console.log("OPEN");
        };
        this.ws.onmessage = (ev) => {
            console.log(ev.data);
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

    attack() {
        this.ws.send(`{"type": "attack"}`);
    }

    offer(gold) {
        this.ws.send(`{"type": "offer",
        "gold": ${gold}}`);
    }

    pass() {
        this.ws.send(`{"type": "pass"}`);
    }

    close() {
        this.ws.close();
    }
}