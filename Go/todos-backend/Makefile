.PHONY: clean
INTERNAL_SRCS:=$(shell find internal -type f -name '*.go')

todos: cmd/todos/main.go $(INTERNAL_SRCS)
	go build -o $@ $<

clean:
	rm -f todos
