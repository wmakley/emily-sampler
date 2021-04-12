package todosapi

import (
	"net/http"

	"github.com/gorilla/mux"
)

func NewRouter() http.Handler {
	router := mux.NewRouter().StrictSlash(true)
	router.Use(middleware)
	router.HandleFunc("/todos", listTodos).Methods(http.MethodGet)
	router.HandleFunc("/todos/{id}", getTodoById).Methods(http.MethodGet)
	router.HandleFunc("/todos", createTodo).Methods(http.MethodPost)
	router.HandleFunc("/todos/{id}", updateTodo).Methods(http.MethodPatch)
	router.HandleFunc("/todos/{id}", deleteTodo).Methods(http.MethodDelete)
	router.HandleFunc("/todos/{id}/check", completeTodo).Methods(http.MethodPatch)
	return router
}

func middleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Add("Content-Type", "application/json; charset=utf-8")
		next.ServeHTTP(w, r)
	})
}
