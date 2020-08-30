CFLAGS := -Wall

all: hello_world curses if

hello_world: hello_world.c
	gcc $(CFLAGS) -o $@ $<

curses: curses.c
	gcc $(CFLAGS) -o $@ -L/usr/lib -lncurses $<

if: if.c
	gcc $(CFLAGS) -o $@ $<

clean:
	rm -f curses
	rm -f hello_world
	rm -f if
