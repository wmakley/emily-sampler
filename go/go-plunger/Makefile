SRCS:=$(shell find . -type f -name '*.go' -not -name '*_test.go')

go-plunger: $(SRCS)
	go build

run: go-plunger
	./go-plunger

.PHONY: run
