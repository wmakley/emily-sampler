SRCS:=$(shell find . -type f -name '*.go')

go-plunger: $(SRCS)
	go build

run: go-plunger
	./go-plunger

.PHONY: run
