package internal

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"
)

var (
	server  *httptest.Server
	client  *http.Client
	counter int
)

func TestTodos(t *testing.T) {
	counter = 0
	ConnectToDB("../testdata/todos_test.sqlite")
	AutoMigrate()
	handler := TodosRouter()
	server = httptest.NewServer(handler)
	defer server.Close()
	client = server.Client()

	t.Run("listTodos", func(t *testing.T) {
		createTestTodo(t)

		url := fmt.Sprintf("%s/todos", server.URL)
		resp, body := Get(url, t)

		assertSuccess(resp, t)
		assertJson(resp, t)
		if !strings.HasPrefix(body, "[") {
			t.Error("body is not a JSON array")
		}

		var todos []Todo
		json.NewDecoder(strings.NewReader(body)).Decode(&todos)

		if len(todos) == 0 {
			t.Error("Todos array is empty")
		}
	})

	t.Run("getTodoById", func(t *testing.T) {
		testTodo := createTestTodo(t)
		// t.Logf("test todo %+v", testTodo)

		url := fmt.Sprintf("%s/todos/%d", server.URL, testTodo.ID)
		resp, body := Get(url, t)

		assertSuccess(resp, t)
		assertJson(resp, t)
		if !strings.HasPrefix(body, "{") {
			t.Error("body is not a JSON object")
		}

		var responseTodo Todo
		json.NewDecoder(strings.NewReader(body)).Decode(&responseTodo)
		if responseTodo.ID != testTodo.ID {
			t.Error("response todo has unexpected ID")
		}
	})
}

func createTestTodo(t *testing.T) Todo {
	todo := Todo{Thing: fmt.Sprintf("Test Todo %d", counter)}
	counter++
	if err := db.Save(&todo).Error; err != nil {
		t.Fatal(err.Error())
	}

	return todo
}

func Get(url string, t *testing.T) (*http.Response, string) {
	t.Log("GET", url)
	resp, err := client.Get(url)
	if err != nil {
		t.Fatal(err.Error())
	}

	bodyBytes, err := io.ReadAll(resp.Body)
	resp.Body.Close()
	if err != nil {
		t.Fatal(err.Error())
	}
	body := strings.TrimSpace(string(bodyBytes))

	t.Log("Status Code:", resp.StatusCode)
	t.Log("Content-Type:", resp.Header.Get("Content-Type"))
	t.Log("Body:", body)

	return resp, body
}

func assertSuccess(resp *http.Response, t *testing.T) {
	if resp.StatusCode != 200 {
		t.Error("Status code is not 200")
	}
}

func assertJson(resp *http.Response, t *testing.T) {
	if resp.Header.Get("Content-Type") != "application/json; charset=utf-8" {
		t.Error("Content-Type is not 'application/json'")
	}
}
