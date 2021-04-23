# Normal go commands work just fine. This simply provides an easy starting point
# for those unfamiliar with go.

.PHONY: clean test
GO_SRCS:=$(shell find . -type f -name '*.go' -and -not -name '*_test.go')

todos: $(GO_SRCS)
	go build -o $@

clean:
	rm -f todos

test:
	go test ./todosapi
