FROM alpine:latest
RUN apk add --no-cache build-base

COPY . /app
WORKDIR /app

RUN make

CMD ["./bin/main"]