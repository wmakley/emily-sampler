package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"strings"
	"time"
	"todos/todosapi"
)

func main() {
	var dbPath string
	var listenAddr string
	var listenPort int
	var delay int

	flag.StringVar(&dbPath, "db", "data/todos.sqlite", "Specify path to sqlite database file.")
	flag.StringVar(&listenAddr, "address", "", "Address to listen on")
	flag.IntVar(&listenPort, "port", 8080, "The port to listen on")
	flag.IntVar(&delay, "delay", 0, "A built-in delay in ms to apply to every response for testing")
	flag.Parse()

	if err := todosapi.ConnectToDB(dbPath); err != nil {
		log.Fatal("Error connecting to database:", err.Error())
	}
	if err := todosapi.AutoMigrate(); err != nil {
		log.Fatal("Error migrating database:", err.Error())
	}

	delayDuration := time.Duration(delay) * time.Millisecond
	api := http.StripPrefix("/api", todosapi.NewRouter())

	// Have to make a custom handler because the default MUX precedence is based on shortest length for some reason
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("%s: %s\n", r.Method, r.RequestURI)

		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "POST, GET, OPTIONS, PATCH, PUT, DELETE")
		w.Header().Set("Access-Control-Allow-Headers", "Accept, Content-Type, Content-Length, Accept-Encoding")

		if r.Method == "OPTIONS" {
			return
		}

		if strings.HasPrefix(r.RequestURI, "/api") {
			time.Sleep(delayDuration)
			api.ServeHTTP(w, r)
			return
		}

		http.NotFound(w, r)
	})

	listenAddrFull := fmt.Sprintf("%s:%d", listenAddr, listenPort)
	fmt.Println("Listening on", listenAddrFull, "with built-in delay of", delay, "ms")
	log.Fatal(http.ListenAndServe(listenAddrFull, nil))
}
