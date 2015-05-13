# Even more changes due to last minute "request" by Gordon
CFLAGS = -I ./include
LINC = -L ./gustavoC/api/lib/x86_64/
LIB    = ./Fonts/libggfonts.so
#LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm $(LINC) -Wl,-rpath=./gustavoC/api/lib/x86_64/ -lfmod#-lXrandr

all: project.cpp Images/ppm.c etc/log.c gustavoC/gustavoC.cpp gabbyP/gabbyP.cpp
	g++ $(CFLAGS) project.cpp etc/log.c gustavoC/gustavoC.cpp Images/ppm.c gabbyP/gabbyP.cpp -Wall -Wextra $(LFLAGS) $(LIB) -o project -w

#all: project.cpp Images/ppm.c etc/log.c
#	g++ $(CFLAGS) project.cpp etc/log.c -Wall -Wextra $(LFLAGS) -o project -w

clean:
	rm -f project
	rm -f *.o

