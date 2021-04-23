<template>
  <li>
    <input type="checkbox" :checked="isComplete" @click="toggleTodo" />
    <span v-if="isEditing">
      <input type="text" :value="todo.thing" />
      <button type="button" @click="doneEditing">Done</button>
    </span>
    <span v-else @click="startEditing">{{ todo.thing }}</span>
    <button type="button" @click="deleteTodo">Delete</button>
  </li>
</template>

<script>
export default {
  name: "Todo",

  props: {
    todo: {
      id: Number,
      thing: String,
      completedAt: Date,
    },
  },

  inject: ["todosStore"],

  data() {
    return {
      isEditing: false,
    };
  },

  computed: {
    isComplete() {
      return this.todo.completedAt !== null;
    },
  },

  methods: {
    toggleTodo() {
      this.todosStore.toggleTodo(this.todo.id);
    },

    deleteTodo() {
      this.todosStore.deleteTodo(this.todo.id);
    },

    startEditing() {
      this.isEditing = true;
    },

    doneEditing() {
      this.isEditing = false;
    },
  },
};
</script>
