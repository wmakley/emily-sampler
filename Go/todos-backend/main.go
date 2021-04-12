package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"todos/todosapi"
)

func main() {
	var dbPath string
	var listenAddr string
	var listenPort int

	flag.StringVar(&dbPath, "db", "data/todos.sqlite", "Specify path to sqlite database file.")
	flag.StringVar(&listenAddr, "address", "", "Address to listen on")
	flag.IntVar(&listenPort, "port", 8080, "The port to listen on")

	todosapi.ConnectToDB(dbPath)
	todosapi.AutoMigrate()

	router := todosapi.NewRouter()

	listenAddrFull := fmt.Sprintf("%s:%d", listenAddr, listenPort)
	fmt.Println("Listening on address: ", listenAddrFull)
	log.Fatal(http.ListenAndServe(listenAddrFull, router))
}
