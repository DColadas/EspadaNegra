<template>
  <div class="hello">
    <div>
      <button @click="processAttack">Attack</button>
      <p>Result: {{ resultAttack }}</p>
    </div>
  </div>
</template>

<script>
import createModule from "../../public/espadaNegra";
let moduleInstance = null;
export default {
  name: "HelloWorld",
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
      let att = new moduleInstance.toEvent(`{"type": "attack", "nickname": "X123XX"}`);
      this.resultAttack = moduleInstance.dispatch(att);
      att.delete();
    },
  },
  beforeCreate() {
    if (moduleInstance === null) {
      createModule().then((myModule) => {
        moduleInstance = myModule;
      });
    }
  },
};
</script>

<style scoped>
</style>
