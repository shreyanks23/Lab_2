CC = gcc

all: client host

client: 
	$(CC) client.c snappy.c tiny_file.c -o client -pthread 

host: 
	$(CC) server.c snappy.c tiny_file.c -o server -pthread

clean:
	rm *.o
