CC = gcc
CFLAGS = -Wall -g -I.
OBJFILES = main.o request.o response.o
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(OBJFILES) -lpthread -o $(TARGET)

main.o: main.c request.h response.h
	$(CC) $(CFLAGS) -c main.c

request.o: request.c request.h response.h
	$(CC) $(CFLAGS) -c request.c

response.o: response.c response.h
	$(CC) $(CFLAGS) -c response.c

run: $(TARGET)
	./$(TARGET) -p 8080

clean:
	rm -f $(OBJFILES) $(TARGET)
