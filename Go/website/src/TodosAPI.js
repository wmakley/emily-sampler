/**
 * Fetch all todos.
 *
 * @returns {Promise<Array<Todo>>}
 */
export function listTodos() {
  return fetch("/api/todos").then(r => r.json())
}

/**
 *
 * @param {object} todo the todo to save
 * @returns {Promise<object>} the created todo
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

/**
 * Update the label of a todo
 *
 * @param {object} todo the todo to update
 * @returns {object} the updated todo
 */
export async function updateTodo(todo) {
  if (typeof todo.id !== "number" || isNaN(todo.id)) {
    throw new Error("todo does not have id");
  }

  const [isValid, errors] = validateTodo(todo);
  if (!isValid) {
    throw new Error("Invalid Todo: " + errors.join(", "));
  }

  const body = JSON.stringify(todo);

  const response = await fetch("/api/todos/" + encodeURIComponent(todo.id), {
    method: "PATCH",
    body: body,
    headers: {
      "Content-Type": "application/json"
    }
  });
  const json = await response.json();
  return json;
}

/**
 *
 * @param {number} id
 * @returns {Promise<Response>}
 */
export function deleteTodo(id) {
  if (typeof id !== "number") {
    throw new Error("id must be a number")
  }

  return fetch("/api/todos/" + encodeURIComponent(id), {
    method: "DELETE"
  });
}

/**
 *
 * @param {number} id
 * @returns {Promise<object>}
 */
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

  if (typeof todo !== "object" || !todo) {
    errors.push("todo must be a non-null object");
  }

  if (typeof todo.thing !== "string") {
    errors.push("todo.thing must be a string");
  } else if (todo.thing.trim() === "") {
    errors.push("todo.thing must not be blank");
  }

  return [errors.length === 0, errors]
}
