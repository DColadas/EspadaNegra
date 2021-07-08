<template>
  <div>
    <div>
      <button @click="processAttack">Attack</button>
      <p>Result: {{ resultAttack }}</p>
      <button @click="processInfo">Info</button>
      <p>Result: {{ resultInfo }}</p>
    </div>
  </div>
</template>

<script>
export default {
  name: "Match",
  props: {
    msg: String,
  },
  data() {
    return {
      resultAttack: null,
      resultInfo: null,
    };
  },
  methods: {
    processAttack() {
      let att = new this.$wasm.toEvent(
        `{"type": "attack", "nickname": "X123XX"}`
      );
      this.resultAttack = this.$wasm.dispatch(att);
      att.delete();
    },
    processInfo() {
      let info = this.$wasm.newMatch(
        `{"numPlayers": 3}`,
        `{"cards": [{"id": 1}, {"id": 2}]}`
      );
      this.resultInfo = info;
      console.log(`RESULT: ${info}`);
    },
  },
};
</script>

<style scoped>
</style>
