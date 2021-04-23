package main

import (
	"flag"
	"fmt"
	"log"
	"net/http"
	"net/url"
	"strings"
	"time"
	"todos/todosapi"
	"todos/webpack_proxy"
)

func main() {
	var dbPath string
	var listenAddr string
	var listenPort int
	var rawWebpackUrl string
	var delay int

	flag.StringVar(&dbPath, "db", "data/todos.sqlite", "Specify path to sqlite database file.")
	flag.StringVar(&listenAddr, "address", "", "Address to listen on")
	flag.IntVar(&listenPort, "port", 8080, "The port to listen on")
	flag.StringVar(&rawWebpackUrl, "webpack-url", "http://localhost:9000", "The full url to proxy webpack requests to")
	flag.IntVar(&delay, "delay", 0, "A built-in delay in ms to apply to every response for testing")
	flag.Parse()

	var err error
	var webpackUrl *url.URL
	webpackUrl, err = url.Parse(rawWebpackUrl)
	if err != nil {
		log.Fatal("Error parsing webpack-url:", err.Error())
	}

	if err = todosapi.ConnectToDB(dbPath); err != nil {
		log.Fatal("Error connecting to database:", err.Error())
	}
	if err = todosapi.AutoMigrate(); err != nil {
		log.Fatal("Error migrating database:", err.Error())
	}

	api := http.StripPrefix("/api", todosapi.NewRouter())
	wpProxy := webpack_proxy.NewWebpackProxy(webpackUrl)

	delayDuration := time.Duration(delay) * time.Millisecond

	// Have to make a custom handler because the default MUX precedence is based on shortest length for some reason
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("%s: %s\n", r.Method, r.RequestURI)

		if strings.HasPrefix(r.RequestURI, "/api") {
			if delayDuration > 0 {
				time.Sleep(delayDuration)
			}
			api.ServeHTTP(w, r)
		} else {
			wpProxy(w, r)
		}
	})

	listenAddrFull := fmt.Sprintf("%s:%d", listenAddr, listenPort)
	fmt.Println("Listening on", listenAddrFull, "with built-in delay of", delay, "ms")
	log.Fatal(http.ListenAndServe(listenAddrFull, nil))
}
