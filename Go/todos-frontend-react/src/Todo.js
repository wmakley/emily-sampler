export default function Todo(props) {
  return (
    <div onClick={props.onClick}>{props.todo.thing}</div>
  )
}
