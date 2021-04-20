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

func ListTodos(w http.ResponseWriter, r *http.Request) {
	fmt.Println("listTodos")

	var todos []Todo

	if err := db.Find(&todos).Error; err != nil {
		internalServerError(err, w)
		return
	}

	encodeJsonOrPanic(w, todos)
}

func GetTodoById(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("getTodoById", key)

	var todo Todo
	if err := db.First(&todo, key).Error; err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound(w, "Todo", key)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(w, todo)
}

func CreateTodo(w http.ResponseWriter, r *http.Request) {
	var todo Todo

	if err := strictDecodeJson(r.Body, &todo); err != nil {
		badRequest(w, err.Error())
		return
	}

	fmt.Printf("createTodo %+v\n", todo)

	todo.CompletedAt = nil

	if err := db.Create(&todo).Error; err != nil {
		internalServerError(err, w)
		return
	}

	encodeJsonOrPanic(w, todo)
}

func DeleteTodo(w http.ResponseWriter, r *http.Request) {
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
			notFound(w, "Todo", key)
		} else {
			internalServerError(err, w)
		}
		return
	}

	w.WriteHeader(http.StatusNoContent)
}

func UpdateTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]

	var jsonTodo Todo
	if err := strictDecodeJson(r.Body, &jsonTodo); err != nil {
		badRequest(w, err.Error())
		return
	}

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
			notFound(w, "Todo", key)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(w, todo)
}

func CompleteTodo(w http.ResponseWriter, r *http.Request) {
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
			notFound(w, "Todo", key)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(w, todo)
}

func ToggleTodo(w http.ResponseWriter, r *http.Request) {
	vars := mux.Vars(r)
	key := vars["id"]
	fmt.Println("toggleTodo", key)

	var todo Todo
	err := db.Transaction(func(tx *gorm.DB) error {
		if err := db.First(&todo, key).Error; err != nil {
			return err
		}

		if todo.CompletedAt == nil {
			timestamp := time.Now()
			todo.CompletedAt = &timestamp
		} else {
			todo.CompletedAt = nil
		}

		if err := db.Save(&todo).Error; err != nil {
			return err
		}

		return nil
	})
	if err != nil {
		if err == gorm.ErrRecordNotFound {
			notFound(w, "Todo", key)
		} else {
			internalServerError(err, w)
		}
		return
	}

	encodeJsonOrPanic(w, todo)
}

func strictDecodeJson(r io.Reader, v interface{}) error {
	decoder := json.NewDecoder(r)
	decoder.DisallowUnknownFields()
	return decoder.Decode(v)
}

// A JSON encoding failure is basically unrecoverable and should never happen
func encodeJsonOrPanic(w io.Writer, v interface{}) {
	err := json.NewEncoder(w).Encode(v)
	if err != nil {
		panic("Error encoding JSON: " + err.Error())
	}
}

func badRequest(w http.ResponseWriter, msg string) {
	w.WriteHeader(http.StatusBadRequest)
	body := ErrorJson{http.StatusBadRequest, msg}
	encodeJsonOrPanic(w, body)
}

func notFound(w http.ResponseWriter, objectName string, id string) {
	w.WriteHeader(http.StatusNotFound)
	body := ErrorJson{http.StatusNotFound, fmt.Sprintf("%s with ID %s not found.", objectName, id)}
	encodeJsonOrPanic(w, body)
}

func internalServerError(err error, w http.ResponseWriter) {
	w.WriteHeader(http.StatusInternalServerError)
	body := ErrorJson{http.StatusInternalServerError, err.Error()}
	encodeJsonOrPanic(w, body)
}
