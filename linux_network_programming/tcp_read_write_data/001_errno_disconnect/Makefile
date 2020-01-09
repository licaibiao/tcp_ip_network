
CC = gcc
all:client server
	
client:001_client_tcp.o
	$(CC) -o client 001_client_tcp.o
server: 001_server_tcp.o
	$(CC) -o server  001_server_tcp.o
clean:
	rm -f client server *.o