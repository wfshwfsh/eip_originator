TAR = main

CC=gcc
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o, $(SRCS))

all: $(OBJS)
	$(CC) -o $(TAR).app $(OBJS)

.depend: $(SRCS)
	$(CC) -MM $(SRCS) > $@

sinclude .depend

clean:
	rm -rf *.o .depend *.app

install:
	cp ./* /mnt/share/eip/examples/tcp/

time = $(shell date +%F)
release: clean
	mkdir -p `pwd`/../bak/$(time)
	cp * `pwd`/../bak/$(time)
