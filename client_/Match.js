import { Player } from "./Player.js";

export class Match {
    constructor(config, players = {}, deck = {}) {
        this.config = config;
        this.players = players; // {nickname: Player}
        this.deck = deck; // {cardID: Card}
        this.inPlay = []
        this.currentAttack = 0;
        this.currentOffer = 0;
    }

    addPlayer(nickname) {
        this.players[nickname] = new Player(nickname);
    }

    draw(cardID) {
        this.inPlay.push(this.deck[cardID]);
    }

    earn(nickname, gold) {
        this.players[nickname].earn(gold);
    }

    getCard(nickname) {
        // Gets the first card in play
        this.players[nickname].getCard(this.inPlay.shift());
    }

    setAuctioneer(nickname) {
        Object.keys(this.players).forEach(key => {
            this.players[key].isAuctioneer = false;
        });
        this.players[nickname].setAuctioneer();
    }

    attack(nickname) {
        this.currentAttack = this.players[nickname].getTotalAttack();
        this.players[nickname].attack();
    }

    offer(nickname, gold) {
        this.currentOffer = gold;
        this.players[nickname].offer(gold);
    }

    pass(nickname) {
        this.players[nickname].pass();
    }

    pay(nickname, gold) {
        this.players[nickname].pay(gold);
    }

    setGold(nickname, gold) {
        this.players[nickname].setGold(gold);
    }

}
