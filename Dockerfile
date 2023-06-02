FROM alpine:latest

WORKDIR /src

VOLUME [ "/src" ]

COPY . /src

RUN apk update && apk add gcc
RUN apk add python3
RUN apk add py3-pip
RUN apk add g++
RUN apk add make
RUN apk add gdb
