<template>
  <div id="app">
    <ConnectionForm @connectionData="joinMatch" />
    <Match />
  </div>
</template>

<script>
import Match from "./components/Match.vue";
import ConnectionForm from "./components/ConnectionForm.vue";
import { ClientWS } from "./js/ClientWS";
import { EventDispatcher } from "./js/EventDispatcher";

let dispatcher = new EventDispatcher();
let ws = null;
let match = null;
export default {
  name: "App",
  components: {
    Match,
    ConnectionForm,
  },
  methods: {
    joinMatch(data) {
      ws = new ClientWS(data.url, dispatcher);
      ws.ready().then(() => {
        ws.joinMatch(data.matchID, data.nickname);
      });
    },
  },
  created() {
    dispatcher.onMatchInfo = (config, players, deck) => {
      match = this.$wasm.newMatch(
        JSON.stringify(config),
        JSON.stringify(players),
        JSON.stringify(deck)
      );
      console.log(`running: ${match.isRunning()}`);
    };
  },
  beforeDestroy() {
    ws.close();
  },
};
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
