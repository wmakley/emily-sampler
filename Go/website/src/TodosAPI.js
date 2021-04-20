/**
 *
 * @returns {Promise<Array<Todo>>}
 */
export function listTodos() {
  console.log("listTodos")
  return fetch("/api/todos").then(r => r.json())
}

/**
 *
 * @param {object} todo
 */
export function createTodo(todo) {
  const [isValid, errors] = validateTodo(todo);
  if (!isValid) {
    throw new Error("Invalid Todo: " + errors.join(", "));
  }

  const body = JSON.stringify(todo);

  return fetch("/api/todos", {
    method: "POST",
    body: body,
    headers: {
      "Content-Type": "application/json"
    }
  }).then(r => r.json());
}

export function deleteTodo(id) {
  if (typeof id !== "number") {
    throw new Error("id must be a number")
  }

  return fetch("/api/todos/" + encodeURIComponent(id), {
    method: "DELETE"
  });
}

export function toggleTodo(id) {
  if (typeof id !== "number") {
    throw new Error("id must be a number")
  }

  return fetch("/api/todos/" + encodeURIComponent(id) + "/toggle", {
    method: "PATCH"
  }).then(r => r.json());
}

/**
 * Validate a Todo, returning a tuple of whether the todo is valid, and any errors.
 *
 * @param {object} todo
 * @returns {[true|false, Array<string>]}
 */
export function validateTodo(todo) {
  const errors = []

  if (!todo) {
    errors.push("todo must be an object");
  }

  if (!todo.thing) {
    errors.push("todo.thing is a required field");
  }

  if (typeof todo.thing !== "string") {
    errors.push("todo.thing must be a non-empty string")
  }

  return [errors.length === 0, errors]
}
