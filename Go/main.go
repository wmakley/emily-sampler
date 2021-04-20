package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"todos/todosapi"
	"todos/webpack_proxy"
)

func main() {
	var dbPath string
	var listenAddr string
	var listenPort int
	var webpackPort int

	flag.StringVar(&dbPath, "db", "data/todos.sqlite", "Specify path to sqlite database file.")
	flag.StringVar(&listenAddr, "address", "", "Address to listen on")
	flag.IntVar(&listenPort, "port", 8080, "The port to listen on")
	flag.IntVar(&webpackPort, "webpackport", 9000, "The port webpack-dev-server is listening on")

	if err := todosapi.ConnectToDB(dbPath); err != nil {
		log.Fatal("Error connecting to database:", err.Error())
	}
	if err := todosapi.AutoMigrate(); err != nil {
		log.Fatal("Error migrating database:", err.Error())
	}

	router := todosapi.NewRouter("/api")
	wpProxy := webpack_proxy.NewWebpackProxy(webpackPort)
	router.HandleFunc("/", wpProxy)
	router.HandleFunc("/{asset}", wpProxy)

	listenAddrFull := fmt.Sprintf("%s:%d", listenAddr, listenPort)
	fmt.Println("Listening on", listenAddrFull)
	log.Fatal(http.ListenAndServe(listenAddrFull, router))
}