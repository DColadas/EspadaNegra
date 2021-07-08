import Vue from 'vue'
import App from './App.vue'
import createModule from "../public/espadaNegraClient";


Vue.config.productionTip = false

createModule().then((myModule) => {
  myModule.init();
  Vue.prototype.$wasm = myModule;
});

new Vue({
  render: h => h(App),
}).$mount('#app')
