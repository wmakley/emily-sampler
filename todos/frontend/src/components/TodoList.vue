<template>
  <div class="container m-4 mx-auto border-4 rounded shadow-lg">
    <h1 class="m-4 text-lg">Tasks</h1>
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
    list: Array,
  },

  components: {
    Todo,
    NewTodo,
  },
};
</script>
