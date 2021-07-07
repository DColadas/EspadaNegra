<template>
  <div>
    <div>
      <button @click="processAttack">Attack</button>
      <p>Result: {{ resultAttack }}</p>
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
  },
};
</script>

<style scoped>
</style>
