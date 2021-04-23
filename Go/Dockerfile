FROM golang:1.16.3-alpine

RUN apk add build-base

RUN mkdir /app
COPY . /app
WORKDIR /app

RUN go mod download

# RUN make test
RUN make

# 2nd stage
FROM alpine:latest

RUN mkdir -p /app/data
WORKDIR /app
COPY --from=0 /app/todos /app/todos

EXPOSE 8080
CMD ["/app/todos"]
