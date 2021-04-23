<template>
  <li :class="isDeleting ? 'text-gray-300' : ''">
    <input
      type="checkbox"
      :checked="isComplete"
      @click="toggleTodo"
      class="mr-2"
    />
    <form v-if="isEditing" @submit.prevent="doneEditing" class="inline-block">
      <input type="text" v-model="todo.thing" ref="input" />
      <button type="submit">Done</button>
    </form>
    <span
      v-else
      @click="startEditing"
      :class="isSaving ? 'text-gray-300' : ''"
      >{{ todo.thing }}</span
    >
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
      isSaving: false,
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
      if (this.isDeleting || this.isSaving) {
        return;
      }
      this.isEditing = true;

      this.$nextTick(() => {
        this.$refs.input.focus()
      })
    },

    async doneEditing() {
      if (this.isDeleting) {
        return;
      }

      this.isSaving = true;
      this.isEditing = false;

      this.todo.thing = this.todo.thing.trim();

      await this.todosStore.updateTodo({
        id: this.todo.id,
        thing: this.todo.thing,
      });

      this.isSaving = false;
    },
  },
};
</script>
