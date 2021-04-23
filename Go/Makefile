.PHONY: clean test run watch

GO_SRCS:=$(shell find . -type f -name '*.go' -and -not -name '*_test.go')

todos: $(GO_SRCS)
	go build -o $@

clean:
	rm -f todos

run: todos
	./todos $(SERVER_FLAGS)

watch:
	reflex -s -r '\.go$$' make run

test:
	go test ./todosapi
