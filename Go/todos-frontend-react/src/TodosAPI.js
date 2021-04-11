/**
 *
 * @returns {Promise<object[]>}
 */
export function listTodos() {
  return fetch("/api/todos", {
    "Accept": "application/json"
  }).then(response => response.json())
}

/**
 *
 * @param {object} todo
 * @returns {Promise<object>}
 */
export function createTodo(todo) {
  if (!todo) {
    throw new Error("Todo must be an object")
  }
  const body = JSON.stringify(todo)

  return fetch("/api/todos", {
    headers: {
			"Content-Type": "application/json",
			"Accept": "application/json"
		},
    method: "POST",
    body: body
  }).then(response => response.json())
}

/**
 *
 * @param {number} id
 * @returns {Promise<Response>}
 */
 export function deleteTodo(id) {
  if (typeof id !== "number" || isNaN(id)) {
    throw new Error("id is not a number")
  }

  return fetch("/api/todos/" + encodeURIComponent(id), {
    headers: {
			"Content-Type": "application/json",
			"Accept": "application/json"
		},
    method: "DELETE"
  })
}
