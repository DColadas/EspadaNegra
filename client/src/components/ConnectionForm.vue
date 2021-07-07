<template>
  <form ref="joinForm" @submit="joinMatch">
    <label
      >Server URL
      <input type="text" v-model="url" />
    </label>
    <label
      >Nickname
      <input type="text" v-model="nickname" />
    </label>
    <label
      >Match ID
      <input type="text" v-model="matchID" />
    </label>
    <button>Join match</button>
  </form>
</template>

<script>
export default {
  name: "ConnectionForm",
  data() {
    return {
      url: "ws://localhost:8081",
      nickname: "a",
      matchID: "1",
    };
  },
  methods: {
    joinMatch(e) {
      e.preventDefault();
      this.$emit("connectionData", {
        url: this.url,
        nickname: this.nickname,
        matchID: this.matchID,
      });
    },
  },
};
</script>

<style>
</style>