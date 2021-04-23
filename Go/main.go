package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"strings"
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

	api := http.StripPrefix("/api", todosapi.NewRouter())
	wpProxy := webpack_proxy.NewWebpackProxy(webpackPort)

	// Have to make a custom handler because the default MUX precedence is based on length for some reason
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("%s: %s\n", r.Method, r.RequestURI)
		if strings.HasPrefix(r.RequestURI, "/api") {
			api.ServeHTTP(w, r)
		} else {
			wpProxy(w, r)
		}
	})

	listenAddrFull := fmt.Sprintf("%s:%d", listenAddr, listenPort)
	fmt.Println("Listening on", listenAddrFull)
	log.Fatal(http.ListenAndServe(listenAddrFull, nil))
}
