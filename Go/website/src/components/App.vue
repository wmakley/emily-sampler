<template>
  <div class="container m-4 mx-auto border-4 rounded shadow-lg">
    <h1 class="m-4">Todos</h1>
    <ul class="m-4">
      <li v-for="(todo, index) in todos" v-bind:key="todo.id">
        <input
          type="checkbox"
          :checked="todoIsComplete(index)"
          @click="toggleTodo(index)"
        />
        {{ todo.thing }}
        <button type="button" @click="deleteTodo(todo.id)">Delete</button>
      </li>
    </ul>
    <form class="border-t-4 p-4" @submit.prevent="addTodo">
      <label for="new-task">New Task</label>
      <input
        type="text"
        id="new-task"
        v-model="newTask"
        placeholder="New Task"
      />
      <button type="submit">Add</button>
    </form>
  </div>
</template>

<script>
import * as TodosAPI from "../TodosAPI.js";

export default {
  mounted() {
    TodosAPI.listTodos().then((todos) => {
      this.todos = todos;
    });
  },

  data() {
    return {
      newTask: "",
      nextTempId: -1,
      todos: [],
    };
  },

  methods: {
    todoIsComplete(index) {
      return this.todos[index].completedAt !== null;
    },

    toggleTodo(index) {
      const todo = this.todos[index];
      if (!todo) {
        throw new Error("todo index " + index + " doesn't exist");
      }

      if (todo.completedAt) {
        todo.completedAt = null;
      } else {
        todo.completedAt = new Date();
      }

      TodosAPI.toggleTodo(todo.id).then((toggledTodo) => {
        // Update the todo by its ID in case the index is no longer valid
        this.todos = this.todos.map((oldTodo) => {
          if (oldTodo.id === todo.id) {
            return toggledTodo;
          } else {
            return oldTodo;
          }
        });
      });
    },

    addTodo() {
      const trimmed = this.newTask.trim();
      this.newTask = trimmed;
      if (trimmed === "") {
        return;
      }

      const todo = {
        thing: this.newTask,
      };

      TodosAPI.createTodo(todo).then((createdTodo) => {
        this.todos.push(createdTodo);
        this.newTask = "";
      });
    },

    deleteTodo(id) {
      // Delete the todo from the array by its ID in case the index is no longer valid
      TodosAPI.deleteTodo(id).then(() => {
        this.todos = this.todos.filter((todo) => todo.id !== id);
      });
    },
  },
};
</script>
