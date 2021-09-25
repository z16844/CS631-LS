CFLAGS=-Wall -Werror -Wextra -std=c99
OBJS=main.o 
TARGET=ls

all: $(TARGET)
clean: 
	rm -f *.o
	rm -f ./bin

$(TARGET): $(OBJS)
	mkdir -p ./bin
	gcc -o ./bin/$@ $(OBJS)

main.o: main.c