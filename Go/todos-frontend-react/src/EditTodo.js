import { useState } from 'react';

export default function EditTodo(props) {
  const [thing, setThing] = useState(props.todo.thing)

  const id = `edit-todo-${props.todo.id}`

  return (
    <form className="form-inline"
          id={`${id}-form`}
          onSubmit={(e) => {
            e.preventDefault()
            props.onSubmit(thing)
            setThing(props.todo.thing)
          }}>
      <div className="form-group mb-2">
        <label className="sr-only" htmlFor={id}>
          Edit Todo
        </label>
        <input type="text"
               placeholder="New Todo"
               id={id}
               value={thing}
               onInput={(e) => setThing(e.currentTarget.value)}
               className="form-control" />
      </div>
    </form>
  )
}
