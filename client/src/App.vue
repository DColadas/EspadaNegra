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

let ws = null;
export default {
  name: "App",
  components: {
    Match,
    ConnectionForm,
  },
  methods: {
    joinMatch(data) {
      ws = new ClientWS(data.url);
      ws.ready().then(() => {
        ws.joinMatch(data.nickname, data.matchID);
      });
    },
  },
  beforeDestroy() {
    ws.close();
  }
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
