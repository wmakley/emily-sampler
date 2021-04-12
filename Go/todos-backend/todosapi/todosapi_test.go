package todosapi

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
	testServer        *httptest.Server
	testClient        *http.Client
	testRecordCounter int
)

func TestTodos(t *testing.T) {
	testRecordCounter = 0
	ConnectToDB("../testdata/todos_test.sqlite")
	AutoMigrate()
	handler := NewRouter()
	testServer = httptest.NewServer(handler)
	defer testServer.Close()
	testClient = testServer.Client()

	t.Run("listTodos", func(t *testing.T) {
		createTestTodo(t)

		url := "/todos"
		resp, body := Get(url, t)

		assertSuccess(resp, t)
		assertJson(resp, t)
		assertJsonArray(body, t)

		var todos []Todo
		json.Unmarshal(body, &todos)
		// json.NewDecoder(strings.NewReader(body)).Decode(&todos)

		if len(todos) == 0 {
			t.Error("Todos array is empty")
		}
	})

	t.Run("getTodoById", func(t *testing.T) {
		testTodo := createTestTodo(t)
		// t.Logf("test todo %+v", testTodo)

		url := fmt.Sprintf("/todos/%d", testTodo.ID)
		resp, body := Get(url, t)

		assertSuccess(resp, t)
		assertJson(resp, t)
		assertJsonObject(body, t)

		var responseTodo Todo
		json.Unmarshal(body, &responseTodo)
		// json.NewDecoder(strings.NewReader(body)).Decode(&responseTodo)
		if responseTodo.ID != testTodo.ID {
			t.Error("response todo has unexpected ID")
		}
	})

	t.Run("createTodo", func(t *testing.T) {
		url := "/todos"
		body := "{\"thing\":\"Test Todo\"}"
		resp, respBody := SendTestHttpRequest("POST", url, body, t)

		assertSuccess(resp, t)
		assertJson(resp, t)
		// assertJsonObject(respBody, t)

		var responseTodo Todo
		if err := json.Unmarshal(respBody, &responseTodo); err != nil {
			t.Fatal(err.Error())
		}
		// json.NewDecoder(strings.NewReader(body)).Decode(&responseTodo)

		if responseTodo.ID <= 0 {
			t.Errorf("response todo has invalid ID: %+v", responseTodo)
		}

		if responseTodo.Thing != "Test Todo" {
			t.Errorf("response todo has unexpected 'Thing' value: %s", responseTodo.Thing)
		}

		if responseTodo.CompletedAt != nil {
			t.Error("response todo should not be completed")
		}
	})

	t.Run("updateTodo", func(t *testing.T) {
		todo := createTestTodo(t)
		url := fmt.Sprintf("/todos/%d", todo.ID)
		body := "{\"thing\":\"Updated Todo\"}"

		resp, respBody := SendTestHttpRequest("PATCH", url, body, t)

		assertSuccess(resp, t)
		assertJson(resp, t)

		var responseTodo Todo
		if err := json.Unmarshal(respBody, &responseTodo); err != nil {
			t.Fatal(err.Error())
		}

		t.Logf("response todo: %+v", responseTodo)

		if responseTodo.ID != todo.ID {
			t.Error("response todo ID does not match original ID")
		}

		if responseTodo.Thing != "Updated Todo" {
			t.Error("response todo 'thing' json does not match user input")
		}

		if err := db.Find(&responseTodo, todo.ID).Error; err != nil {
			t.Fatalf("Error loading updated Todo: %s", err.Error())
		}

		if responseTodo.Thing != "Updated Todo" {
			t.Error("response todo 'thing' was not updated")
		}
	})
}

func createTestTodo(t *testing.T) Todo {
	todo := Todo{Thing: fmt.Sprintf("Test Todo %d", testRecordCounter)}
	testRecordCounter++
	if err := db.Save(&todo).Error; err != nil {
		t.Fatal(err.Error())
	}

	return todo
}

func Get(url string, t *testing.T) (*http.Response, []byte) {
	url = normalizeUrl(url)

	t.Log("GET", url)
	resp, err := testClient.Get(url)
	if err != nil {
		t.Fatal(err.Error())
	}

	respBody, err := io.ReadAll(resp.Body)
	resp.Body.Close()
	if err != nil {
		t.Fatal(err.Error())
	}

	t.Log("Response Status Code:", resp.StatusCode)
	t.Log("Response Content-Type:", resp.Header.Get("Content-Type"))
	t.Log("Response Body:", respBody)

	return resp, respBody
}

func SendTestHttpRequest(method string, url string, body string, t *testing.T) (*http.Response, []byte) {
	url = normalizeUrl(url)
	t.Log(method, url)

	req, err := http.NewRequest(method, url, strings.NewReader(body))
	if err != nil {
		t.Fatal(err.Error())
	}

	resp, err := testClient.Do(req)
	if err != nil {
		t.Fatal(err.Error())
	}

	responseBodyBytes, err := io.ReadAll(resp.Body)
	resp.Body.Close()
	if err != nil {
		t.Fatal(err.Error())
	}

	t.Log("Response Status Code:", resp.StatusCode)
	t.Log("Response Content-Type:", resp.Header.Get("Content-Type"))
	t.Log("Response Body:", responseBodyBytes)

	return resp, responseBodyBytes
}

func normalizeUrl(url string) string {
	if !strings.HasPrefix(url, testServer.URL) {
		return fmt.Sprintf("%s%s", testServer.URL, url)
	}
	return url
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

func assertJsonArray(body []byte, t *testing.T) {
	if !strings.HasPrefix(string(body), "[") {
		t.Error("body is not a JSON array")
	}
}

func assertJsonObject(body []byte, t *testing.T) {
	if !strings.HasPrefix(string(body), "{") {
		t.Error("body is not a JSON object")
	}
}
