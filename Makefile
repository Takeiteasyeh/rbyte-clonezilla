OBJS	= src/rbzilla.o src/commands.o
SOURCE	= src/rbzilla.c src/commands.c
HEADER	= includes/rbzilla.h includes/commands.h
OUT	= rbzilla
CC	 = gcc
FLAGS	 = -std=gnu99 -g -c -Wall
LFLAGS	 = -lpthread
IDIR = includes
CFLAGS = -I$(IDIR)

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) $(CFLAGS)
	@echo RBZilla by Ray Lynk.
	@echo This should run on most *nix systems.

src/rbzilla.o: src/rbzilla.c
	$(CC) $(FLAGS) src/rbzilla.c -o src/rbzilla.o $(CFLAGS)

src/commands.o: src/commands.c
	$(CC) $(FLAGS) src/commands.c -o src/commands.o $(CFLAGS)

clean:
	rm -f $(OBJS) $(OUT)

install:
	install -m 750 rbzilla /usr/sbin/
	@echo Installed to /usr/sbin/

