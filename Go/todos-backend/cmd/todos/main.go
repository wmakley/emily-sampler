package main

import (
	"fmt"
	"log"
	"net/http"
	"todos/internal"
)

func main() {
	internal.ConnectToDB("data/todos.sqlite")
	internal.AutoMigrate()
	router := internal.TodosRouter()
	fmt.Println("Listening on port 8080")
	log.Fatal(http.ListenAndServe(":8080", router))
}
