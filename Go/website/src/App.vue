<template>
  <TodoList :list="todosStore.state.todos" />
</template>

<script>
import TodoList from "./components/TodoList.vue";

export default {
  name: "App",

  components: {
    TodoList,
  },

  inject: ["todosStore"],

  mounted() {
    console.log("app mounted");
    if (!this.todosStore.loaded) {
      this.todosStore.fetchTodos();
    }
  },
};
</script>
