import './index.css';
import { createApp } from 'vue';
import App from './App.vue';
import TodosStore from "./TodosStore";

const app = createApp(App);

const todosStore = new TodosStore();
todosStore.fetchTodos();

app.provide('todosStore', todosStore);

app.mount('#app');
