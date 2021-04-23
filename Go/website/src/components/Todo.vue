<template>
  <li :class="isDeleting ? 'text-gray-300' : ''">
    <input
      type="checkbox"
      :checked="isComplete"
      @click="toggleTodo"
      class="mr-2"
    />
    <span v-if="isEditing">
      <input type="text" :value="todo.thing" />
      <button type="button" @click="doneEditing">Done</button>
    </span>
    <span v-else @click="startEditing">{{ todo.thing }}</span>
    <button type="button" @click="deleteTodo" class="ml-2">Delete</button>
  </li>
</template>

<script>
export default {
  name: "Todo",

  props: {
    todo: {
      id: Number,
      thing: String,
      completedAt: String,
    },
  },

  inject: ["todosStore"],

  data() {
    return {
      isEditing: false,
      isDeleting: false,
    };
  },

  computed: {
    isComplete() {
      return this.todo.completedAt !== null;
    },
  },

  methods: {
    toggleTodo() {
      if (this.isDeleting) {
        return;
      }

      // Preemptively toggle it so user sees no delay
      if (this.todo.completedAt) {
        this.todo.completedAt = null;
      } else {
        this.todo.completedAt = new Date().toISOString();
      }

      this.todosStore.toggleTodo(this.todo.id);
    },

    async deleteTodo() {
      if (this.isDeleting) {
        return;
      }

      this.isDeleting = true;
      await this.todosStore.deleteTodo(this.todo.id);
      this.isDeleting = false;
    },

    startEditing() {
      if (this.isDeleting) {
        return;
      }
      this.isEditing = true;
    },

    doneEditing() {
      this.isEditing = false;
    },
  },
};
</script>
