const URI = "ws://localhost:8080";

var ws = new WebSocket(URI);
ws.onopen = (ev) => {
    console.log("OPEN");
    ws.send(`{"type": "joinMatchRequest", "matchID": "Match1", "nickname": "Luis"}`);
};
ws.onmessage = (ev) => {
    console.log(ev);
};
