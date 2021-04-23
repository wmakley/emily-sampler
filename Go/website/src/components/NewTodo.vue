<template>
  <form class="border-t-4 p-4" @submit.prevent="createTodo">
    <label for="new-task" class="mr-2">New Task</label>
    <input
      type="text"
      id="new-task"
      v-model="newThing"
      placeholder="New Task"
    />
    <button type="submit" class="ml-2">Add</button>
  </form>
</template>

<script>
export default {
  inject: ["todosStore"],

  data() {
    return {
      newThing: "",
    };
  },

  methods: {
    createTodo() {
      this.todosStore.createTodo(this.newThing);
      this.newThing = "";
    },
  },
};
</script>
