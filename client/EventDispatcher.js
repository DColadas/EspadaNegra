export class EventDispatcher {
    constructor() { }

    dispatchEvent(event) {
        const type = event.type;
        switch (type) {
            case "draw":
                this.onDraw(event.card.id);
                break;
            case "matchInfo":
                this.onMatchInfo(event.config, event.players, event.deck);
                break;
            case "getCard":
                this.onGetCard(event.nickname);
                break;
            case "winner":
                this.onWinner(event.nickname);
                break;
            case "pay":
                this.onPay(event.nickname, event.gold);
                break;
            case "earn":
                this.onEarn(event.nickname, event.gold);
                break;
            case "joinMatch":
                this.onJoinMatch(event.nickname);
                break;
            case "attack":
                this.onAttack(event.nickname);
                break;
            case "pass":
                this.onPass(event.nickname);
                break;
            case "offer":
                this.onOffer(event.nickname, event.gold);
                break;
            case "isAuctioneer":
                this.onIsAuctioneer(event.nickname);
                break;
            case "leave":
                this.onLeave(event.nickname, event.reason);
                break;
            case "setGold":
                this.onSetGold(event.nickname, event.gold);
                break;
            case "error":
                this.onError(event.message);
                break;
            case "complex":
                this.onComplex(event.events);
                break;

        }
    }

    onDraw(cardID) {
        console.log("onDraw");
    }

    onMatchInfo(config, players, deck) {
        console.log("onMatchInfo");
    }

    onGetCard(nickname) {
        console.log("onGetCard");
    }

    onWinner(nickname) {
        console.log("onWinner");
    }

    onPay(nickname, gold) {
        console.log("onPay");
    }

    onEarn(nickname, gold) {
        console.log("onEarn");
    }

    onJoinMatch(nickname) {
        console.log("onJoinMatch");
    }

    onAttack(nickname) {
        console.log("onAttack");
    }

    onPass(nickname) {
        console.log("onPass");
    }

    onOffer(gold) {
        console.log("onOffer");
    }

    onIsAuctioneer(nickname) {
        console.log("onIsAuctioneer");
    }

    onLeave(nickname, reason) {
        console.log("onLeave");
    }

    onSetGold(nickname, gold) {
        console.log("onSetGold");
    }

    onError(message) {
        console.log("onError");
    }

    onComplex(events) {
        events.forEach(e => {
            this.dispatchEvent(e);
        });
    }
}