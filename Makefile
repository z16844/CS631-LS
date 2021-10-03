CFLAGS=-Wall -Werror -Wextra -std=c99 -g
OBJS=main.o parameters.o
TARGET=ls

all: $(TARGET)
clean: 
	rm -f *.o
	rm -rf ./bin

$(TARGET): $(OBJS)
	mkdir -p ./bin
	gcc -o ./bin/$@ $(OBJS)

main.o: parameters.h main.c
parameters.o: parameters.h parameters.c