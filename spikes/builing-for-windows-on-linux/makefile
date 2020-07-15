

# install with: sudo apt install gcc-mingw-w64-x86-64
CC = x86_64-w64-mingw32-gcc-win32

CFLAGS += -Wall -Werror --std=c90

main.exe: main.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

main.o: main.c
	$(CC) -c $(NDFLAGS) $(CPPFLAGS) $(CFLAGS) $< -o $@

# sudo apt install wine64
run: main.exe
	wine ./main.exe

clean:
	rm -rvf main.exe main.o
