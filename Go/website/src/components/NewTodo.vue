<template>
  <form class="border-t-4 p-4" @submit.prevent="createTodo">
    <label for="new-task" class="mr-2">New Task</label>
    <span v-if="isSaving">Saving...</span>
    <input
      v-else
      type="text"
      id="new-task"
      v-model="newThing"
      placeholder="New Task"
    />
    <button type="submit" class="ml-2" :disabled="isSaving">Add</button>
  </form>
</template>

<script>
export default {
  inject: ["todosStore"],

  data() {
    return {
      newThing: "",
      isSaving: false,
    };
  },

  methods: {
    async createTodo() {
      if (this.isSaving) {
        return;
      }

      this.isSaving = true;
      await this.todosStore.createTodo(this.newThing);
      this.newThing = "";
      this.isSaving = false;
    },
  },
};
</script>
