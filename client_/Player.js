
export class Player {
    constructor(nickname) {
        this.nickname = nickname;
        this.gold = null;
        this.cards = [];
        this.attacked = false;
        this.hasPassed = false;
        this.isAuctioneer = false;
    }

    attack() {
        this.attacked = false;
    }

    pass() {
        this.hasPassed = true;
    }

    offer(gold) {
    }

    resetAuctionState() {
        this.hasPassed = false;
    }

    setGold(gold) {
        this.gold = gold;
    }

    earn(gold) {
        this.gold += gold;
    }

    pay(gold) {
        this.gold -= gold;
    }

    getCard(card) {
        this.cards.push(card);
    }

    setAuctioneer() {
        this.isAuctioneer = true;
    }

    canAttack(amount) {
        return !this.attacked && !this.hasPassed && this.getTotalAttack() !== 0;
    }

    getTotalAttack() {
        return this.cards.reduce((a, b) => {
            return a + b.attack;
        })
    }
}