package todosapi

import (
	"net/http"

	"github.com/gorilla/mux"
)

func NewRouter() *mux.Router {
	api := mux.NewRouter().StrictSlash(false)
	api.Use(jsonContentType)
	api.HandleFunc("/todos", ListTodos).Methods(http.MethodGet)
	api.HandleFunc("/todos/{id}", GetTodoById).Methods(http.MethodGet)
	api.HandleFunc("/todos", CreateTodo).Methods(http.MethodPost)
	api.HandleFunc("/todos/{id}", UpdateTodo).Methods(http.MethodPatch)
	api.HandleFunc("/todos/{id}", DeleteTodo).Methods(http.MethodDelete)
	api.HandleFunc("/todos/{id}/check", CompleteTodo).Methods(http.MethodPatch)
	api.HandleFunc("/todos/{id}/toggle", ToggleTodo).Methods(http.MethodPatch)
	return api
}

func jsonContentType(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Add("Content-Type", "application/json; charset=utf-8")
		next.ServeHTTP(w, r)
	})
}
