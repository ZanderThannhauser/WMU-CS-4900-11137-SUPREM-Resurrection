

# install with: sudo apt install gcc-mingw-w64-x86-64
CC = x86_64-w64-mingw32-gcc-win32

CFLAGS += -Wall -Werror

main: main.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

main.o: main.c
	$(CC) -c $(NDFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@

# sudo apt install wine64
run: main
	wine ./main

clean:
	rm -rvf main main.o
