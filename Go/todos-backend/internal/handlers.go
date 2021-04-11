package internal

import (
	"encoding/json"
	"fmt"
	"net/http"
	"time"

	"github.com/gorilla/mux"
	"gorm.io/gorm"
)

func listTodos(w http.ResponseWriter, r *http.Request) {
	fmt.Println("listTodos")

	var todos []Todo

	if err := db.Find(&todos).Error; err != nil {
		internalServerError(err, w, r)
		return
	}

	json.NewEncoder(w).Encode(todos)
}

func getTodoById(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("getTodoById", key)

	var todo Todo
	if err := db.First(&todo, key).Error; err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound("Todo", key, w, r)
		} else {
			internalServerError(err, w, r)
		}
		return
	}

	json.NewEncoder(w).Encode(todo)
}

func createTodo(w http.ResponseWriter, r *http.Request) {
	var todo Todo
	json.NewDecoder(r.Body).Decode(&todo)
	fmt.Printf("createTodo %+v\n", todo)

	todo.CompletedAt = nil

	if err := db.Create(&todo).Error; err != nil {
		internalServerError(err, w, r)
		return
	}

	json.NewEncoder(w).Encode(todo)
}

func deleteTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("deleteTodo", key)

	err := db.Transaction(func(tx *gorm.DB) error {
		var todo Todo

		if err := tx.Find(&todo, key).Error; err != nil {
			return err
		}

		if err := tx.Delete(&todo).Error; err != nil {
			return err
		}

		return nil
	})
	if err != nil {
		internalServerError(err, w, r)
		return
	}

	w.WriteHeader(http.StatusNoContent)
}

func updateTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	var jsonTodo Todo
	json.NewDecoder(r.Body).Decode(&jsonTodo)

	fmt.Println("updateTodo", key, jsonTodo)

	var todo Todo
	err := db.Transaction(func(tx *gorm.DB) error {
		if err := db.Find(&todo, key).Error; err != nil {
			return err
		}

		todo.Thing = jsonTodo.Thing
		if err := db.Save(&todo).Error; err != nil {
			return err
		}

		return nil
	})
	if err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound("Todo", key, w, r)
		} else {
			internalServerError(err, w, r)
		}
		return
	}

	json.NewEncoder(w).Encode(todo)
}

func completeTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("completeTodo", key)

	var todo Todo
	err := db.Transaction(func(tx *gorm.DB) error {
		if err := db.Find(&todo, key).Error; err != nil {
			return err
		}

		timestamp := time.Now()
		todo.CompletedAt = &timestamp

		if err := db.Save(&todo).Error; err != nil {
			return err
		}

		return nil
	})
	if err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound("Todo", key, w, r)
		} else {
			internalServerError(err, w, r)
		}
		return
	}

	json.NewEncoder(w).Encode(todo)
}

func notFound(objectName string, id string, w http.ResponseWriter, r *http.Request) {
	w.WriteHeader(http.StatusNotFound)
	body := ErrorJson{404, fmt.Sprintf("%s with ID %s not found.", objectName, id)}
	json.NewEncoder(w).Encode(body)
}

func internalServerError(err error, w http.ResponseWriter, r *http.Request) {
	w.WriteHeader(http.StatusInternalServerError)
	body := ErrorJson{500, err.Error()}
	json.NewEncoder(w).Encode(body)
}
