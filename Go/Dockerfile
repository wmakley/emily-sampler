FROM golang:1.16.3-alpine

RUN apk add build-base

RUN mkdir -p /app/data
COPY . /app/
WORKDIR /app

RUN go get github.com/cespare/reflex
RUN go mod download

# RUN make test
RUN make
CMD ["make", "watch-docker"]
