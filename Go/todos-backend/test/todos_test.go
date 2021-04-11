package test

import (
	"fmt"
	"io"
	"net/http"
	"net/http/httptest"
	"testing"
	"todos/internal"
)

var (
	server *httptest.Server
	client *http.Client
)

func TestTodos(t *testing.T) {
	internal.ConnectToDB("data/todos_test.sqlite")
	internal.AutoMigrate()
	handler := internal.TodosRouter()
	server = httptest.NewServer(handler)
	defer server.Close()
	client = server.Client()

	t.Run("Index", index)
}

func index(t *testing.T) {
	url := fmt.Sprintf("%s/todos", server.URL)
	t.Log("GET", url)

	resp, err := client.Get(url)
	if err != nil {
		t.Fatal(err.Error())
	}

	body, err := io.ReadAll(resp.Body)
	resp.Body.Close()
	if err != nil {
		t.Fatal(err.Error())
	}

	t.Log("Status Code:", resp.StatusCode)
	t.Log("Content-Type:", resp.Header.Get("Content-Type"))
	t.Log("Body:", string(body))

	if resp.StatusCode != 200 {
		t.Error("Status code is not 200")
	}

	if resp.Header.Get("Content-Type") != "application/json; charset=utf-8" {
		t.Error("Content-Type is not 'application/json'")
	}
}
