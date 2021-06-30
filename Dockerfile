# syntax=docker/dockerfile:1
FROM gcc:11
COPY ./source /usr/src/streamfiler
WORKDIR /usr/src/streamfiler
RUN make
