.PHONY: clean test
INTERNAL_SRCS:=$(shell find internal -type f -name '*.go' -and -not -name '*_test.go')

todos: cmd/todos/main.go $(INTERNAL_SRCS)
	go build -o $@ $<

clean:
	rm -f todos

test:
	go test ./internal
