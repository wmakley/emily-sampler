package todosapi

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"time"

	"github.com/gorilla/mux"
	"gorm.io/gorm"
)

func listTodos(w http.ResponseWriter, r *http.Request) {
	fmt.Println("listTodos")

	var todos []Todo

	if err := db.Find(&todos).Error; err != nil {
		internalServerError(err, w)
		return
	}

	encodeJsonOrPanic(todos, w)
}

func getTodoById(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("getTodoById", key)

	var todo Todo
	if err := db.First(&todo, key).Error; err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound("Todo", key, w)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(todo, w)
}

func createTodo(w http.ResponseWriter, r *http.Request) {
	var todo Todo
	json.NewDecoder(r.Body).Decode(&todo)
	fmt.Printf("createTodo %+v\n", todo)

	todo.CompletedAt = nil

	if err := db.Create(&todo).Error; err != nil {
		internalServerError(err, w)
		return
	}

	encodeJsonOrPanic(todo, w)
}

func deleteTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("deleteTodo", key)

	err := db.Transaction(func(tx *gorm.DB) error {
		result := tx.Delete(&Todo{}, key)
		if result.Error != nil {
			return result.Error
		}

		if result.RowsAffected == 0 {
			return gorm.ErrRecordNotFound
		}

		return nil
	})
	if err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound("Todo", key, w)
		} else {
			internalServerError(err, w)
		}
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
		if err := db.First(&todo, key).Error; err != nil {
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
			notFound("Todo", key, w)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(todo, w)
}

func completeTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("completeTodo", key)

	var todo Todo
	err := db.Transaction(func(tx *gorm.DB) error {
		if err := db.First(&todo, key).Error; err != nil {
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
			notFound("Todo", key, w)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(todo, w)
}

// A JSON encoding failure is basically unrecoverable and should never happen
func encodeJsonOrPanic(v interface{}, w io.Writer) {
	if err := json.NewEncoder(w).Encode(v); err != nil {
		panic(err)
	}
}

func notFound(objectName string, id string, w http.ResponseWriter) {
	w.WriteHeader(http.StatusNotFound)
	body := ErrorJson{http.StatusNotFound, fmt.Sprintf("%s with ID %s not found.", objectName, id)}
	if err := json.NewEncoder(w).Encode(body); err != nil {
		// Should never happen, pretty much unrecoverable
		panic(err)
	}
}

func internalServerError(err error, w http.ResponseWriter) {
	w.WriteHeader(http.StatusInternalServerError)
	body := ErrorJson{http.StatusInternalServerError, err.Error()}
	if err := json.NewEncoder(w).Encode(body); err != nil {
		// Should never happen, pretty much unrecoverable
		panic(err)
	}
}
