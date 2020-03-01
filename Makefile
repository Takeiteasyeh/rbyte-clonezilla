OBJS	= src/rbzilla.o src/diskinfo.o src/smart.o
SOURCE	= src/rbzilla.c src/diskinfo.c src/smart.c
HEADER	= includes/rbzilla.h includes/commands.h includes/diskinfo.h includes/smart.h
OUT	= rbzilla
CC	 = gcc
FLAGS	 = -std=gnu99 -g -c -Wall
LFLAGS	 = -lpthread
IDIR = includes
CFLAGS = -I$(IDIR)

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS) $(CFLAGS)
	@echo RBZilla by Ray Lynk.
	@echo This usually needs to be installed with CloneZilla!

src/rbzilla.o: src/rbzilla.c
	$(CC) $(FLAGS) src/rbzilla.c -o src/rbzilla.o $(CFLAGS)

src/diskinfo.o: src/diskinfo.c
	$(CC) $(FLAGS) src/diskinfo.c -o src/diskinfo.o $(CFLAGS)

src/smart.o: src/smart.c
	$(CC) $(FLAGS) src/smart.c -o src/smart.o $(CFLAGS)

clean:
	rm -f $(OBJS) $(OUT)

install:
	install -m 750 rbzilla /usr/sbin/
	@echo Installed to /usr/sbin/

