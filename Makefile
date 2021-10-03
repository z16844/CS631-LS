CFLAGS=-Wall -Werror -Wextra -std=c99 -g
OBJS=main.o parameters.o file_listing.o
TARGET=ls

all: $(TARGET)
clean: 
	rm -f *.o
	rm -rf ./bin

$(TARGET): $(OBJS)
	mkdir -p ./bin
	gcc -o ./bin/$@ $(OBJS)

main.o: file_listing.h parameters.h main.c
parameters.o: parameters.h parameters.c
file_listing.o: file_listing.h file_listing.c