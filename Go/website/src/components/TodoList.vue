<template>
  <div class="container m-4 mx-auto border-4 rounded shadow-lg">
    <h1 class="m-4">Todos</h1>
    <ul class="m-4">
      <Todo v-for="todo in list" :key="todo.id" :todo="todo" />
    </ul>
    <NewTodo />
  </div>
</template>

<script>
import Todo from "./Todo.vue";
import NewTodo from "./NewTodo.vue";

export default {
  name: "TodoList",

  props: {
    list: {
      type: Array,
      default: [],
    },
  },

  components: {
    Todo,
    NewTodo,
  },
};
</script>
