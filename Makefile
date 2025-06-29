CC := gcc
CFLAGS := -std=c17 -O3 -march=native

server: server.c
	${CC} ${CFLAGS} ${?} -o server

client: client.c
	${CC} ${CFLAGS} ${?} -o client

clean:
	rm server
	rm client