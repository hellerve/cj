TARGET=libcj.a
EXAMPLETARGET=simple
BUILDDIR=bin/
PREFIX=/usr/local/lib/
EXAMPLESRC=examples/simple.c
SOURCES=$(wildcard src/*.c)
override CFLAGS+=-std=c11 -O2 -Wno-gnu
SHARED=-shared
DEVFLAGS=-Werror -Wall -g -fPIC -DNDEBUG -Wfloat-equal -Wundef -Wwrite-strings -Wuninitialized -pedantic -fsanitize=address -O0

all:
	mkdir -p $(BUILDDIR)
	$(CC) $(SOURCES) -o $(BUILDDIR)$(TARGET) $(CFLAGS) $(SHARED)

dev:
	mkdir -p $(BUILDDIR)
	$(CC) $(SOURCES) -o $(BUILDDIR)$(TARGET) $(CFLAGS) $(DEVFLAGS) $(SHARED)

example: all
	mkdir -p $(BUILDDIR)
	$(CC) $(EXAMPLESRC) -o $(BUILDDIR)$(EXAMPLETARGET) $(CFLAGS) $(DEVFLAGS) -lcj -L$(BUILDDIR) -Isrc

install: all
	install $(BUILDDIR)$(TARGET) $(PREFIX)$(TARGET)

uninstall:
	rm -rf $(PREFIX)$(TARGET)

clean:
	rm -rf $(BUILDDIR)
