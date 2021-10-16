CFLAGS=-Wall -Werror -Wextra -std=c99 -g -Wno-unused-variable
OBJS=main.o parameters.o file_listing.o misc.o sorting.o display.o
TARGET=ls

all: $(TARGET)
clean: 
	rm -f *.o
	rm -rf ./bin

$(TARGET): $(OBJS)
	mkdir -p ./bin
	gcc -o ./bin/$@ $(OBJS)

main.o: display.h sorting.h file_listing.h parameters.h main.c
parameters.o: misc.h parameters.h parameters.c
file_listing.o: parameters.h misc.h file_listing.h file_listing.c
misc.o: misc.h misc.c
sorting.o: parameters.h file_listing.h sorting.h sorting.c
display.o: parameters.h file_listing.h display.h display.c