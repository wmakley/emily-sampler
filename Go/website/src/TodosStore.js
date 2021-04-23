import { reactive, isReactive, toRaw } from 'vue';
import * as TodosAPI from './TodosAPI';

export default class {

  constructor() {
    this.loaded = false;
    this.state = reactive({
      todos: []
    });
  }

  async fetchTodos() {
    console.log('fetch todos');

    this.loaded = true;
    const todos = await TodosAPI.listTodos();
    this.state.todos = todos;
    return todos;
  }

  async toggleTodo(id) {
    console.log("toggle todo:", id);

    const toggledTodo = await TodosAPI.toggleTodo(id);

    this.state.todos = this.state.todos.map((oldTodo) => {
      if (oldTodo.id === id) {
        return toggledTodo;
      } else {
        return oldTodo;
      }
    });

    return toggledTodo;
  }

  async createTodo(todo) {
    if (isReactive(todo)) {
      todo = {...toRaw(todo)};
    }

    console.log("create todo:", thing);

    const createdTodo = await TodosAPI.createTodo(todo);

    this.state.todos.push(createdTodo);

    return createdTodo;
  }

  async updateTodo(todo) {
    if (isReactive(todo)) {
      todo = {...toRaw(todo)};
    }

    console.log("update todo:", todo);

    const updatedTodo = await TodosAPI.updateTodo(todo);

    this.state.todos = this.state.todos.map((oldTodo) => {
      if (oldTodo.id === todo.id) {
        return updatedTodo;
      } else {
        return oldTodo;
      }
    });

    return updatedTodo;
  }

  async deleteTodo(id) {
    console.log("delete todo:", id);

    const response = await TodosAPI.deleteTodo(id);

    // Delete the todo from the array by its ID in case the index is no longer valid
    this.state.todos = this.state.todos.filter((todo) => todo.id !== id);

    return true;
  }
};
