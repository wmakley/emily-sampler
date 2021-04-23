FROM golang:1.16.3-alpine

RUN apk add build-base

RUN mkdir -p /app/data
COPY . /app/
WORKDIR /app

RUN go mod download
RUN go get github.com/cespare/reflex

RUN make
CMD ["make", "watch-docker"]
