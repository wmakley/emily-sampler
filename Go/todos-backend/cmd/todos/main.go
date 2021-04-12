package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"todos/internal"
)

func main() {
	var db string
	var address string
	var port int

	flag.StringVar(&db, "db", "data/todos.sqlite", "Specify path to sqlite database file.")
	flag.StringVar(&address, "address", "", "Address to listen on")
	flag.IntVar(&port, "port", 8080, "The port to listen on")

	internal.ConnectToDB(db)
	internal.AutoMigrate()

	router := internal.TodosRouter()

	fmt.Println("Listening on port", port)
	log.Fatal(http.ListenAndServe(fmt.Sprintf("%s:%d", address, port), router))
}
