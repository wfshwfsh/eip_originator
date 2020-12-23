SERVER = server
CLIENT = client

CC=gcc


all: server client

server: server.c
	$(CC) $^ -o $@.app

client: client.c
	$(CC) $^ -o $@.app

clean:
	rm -rf *.app

install:
	cp ./* /mnt/share/eip/examples/tcp/
