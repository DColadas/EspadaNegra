import { EventDispatcher } from "./EventDispatcher";

const sleep = async (time) => {
    return new Promise(r => setTimeout(r, time));
};

export class ClientWS {
    constructor(serverURL) {
        this.dispatcher = new EventDispatcher();
        this.ws = new WebSocket(serverURL);
        this.ws.onmessage = (ev) => {
            console.log(ev.data);
            this.dispatcher.dispatchEvent(JSON.parse(ev.data));
        };
    }

    // Returns true when the instance is ready and can be used
    async ready() {
        while (this.ws.readyState === 0) {
            await sleep(100);
        }
        return true;
    }

    joinMatch(matchID, nickname) {
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