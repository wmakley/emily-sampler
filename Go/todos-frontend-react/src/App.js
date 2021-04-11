import './App.css';
import EditTodo from './EditTodo';
import Todo from './Todo'
import React from 'react';
import * as TodosAPI from './TodosAPI'

class App extends React.PureComponent {
  constructor(props) {
    super(props);

    this.state = {
      todos: [],
      nextTempId: -1,
      editingTodo: null
    };
  }

  componentDidMount() {
    this.reloadTodos();
  }

  reloadTodos() {
    TodosAPI.listTodos().then(json => this.setState({
      todos: json
    }));
  }

  render() {
    const newTodo = { id: 0, thing: "" }

    return (
      <div className="container">
        <h1>Todos</h1>
        <div className="table-responsive">
          <table className="table table-bordered table-striped table-hover">
            <thead>
              <tr>
                <th>Thing</th>
                <th><span className="sr-only">Action</span></th>
              </tr>
            </thead>
            <tbody>
              {this.state.todos.map(todo =>
                <tr key={todo.id}>
                  <td>
                    {todo.thing}
                  </td>
                  <td>
                    <button type="button"
                            className="btn btn-danger"
                            onClick={e => {
                              TodosAPI.deleteTodo(todo.id).then(r => {
                                this.reloadTodos()
                              })
                            }}
                    >
                      Delete
                    </button>
                  </td>
                </tr>
              )}
              <tr>
                <td>
                  <EditTodo
                    todo={newTodo}
                    onSubmit={(thing) => {
                      const newTodos = this.state.todos.slice();
                      newTodos.push({
                        id: this.state.nextTempId,
                        thing: thing
                      });

                      TodosAPI.createTodo({ thing: thing })

                      this.setState({
                        todos: newTodos,
                        nextTempId: this.state.nextTempId - 1
                      });
                    }}
                  />
                </td>
                <td>
                  <button className="btn btn-primary">Add</button>
                </td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    );
  }
}

export default App;
