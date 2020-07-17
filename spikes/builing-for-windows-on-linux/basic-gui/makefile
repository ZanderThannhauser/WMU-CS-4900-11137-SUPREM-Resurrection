
CC = x86_64-w64-mingw32-gcc

CFLAGS += -D UNICODE -D _UNICODE

LDLIBS += -lcomctl32 -Wl,--subsystem,windows

main.exe: AboutDialog.o MainWindow.o WinMain.o Resource.o
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.c
	$(CC) -c $< $(CPPFLAGS) $(CFLAGS) -o $@

Resource.o: Resource.rc Application.ico Application.manifest Resource.h
	x86_64-w64-mingw32-windres -i $< -o $@

Application.ico:
	wget https://raw.githubusercontent.com/TransmissionZero/MinGW-Win32-Application/master/res/Application.ico

AboutDialog.o: AboutDialog.c AboutDialog.h Globals.h Resource.h

MainWindow.o: MainWindow.c AboutDialog.h Globals.h MainWindow.h Resource.h

WinMain.o: WinMain.c Globals.h MainWindow.h Resource.h

