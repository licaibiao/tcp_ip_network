
CC = gcc
all:client server
	
client:01_client_tcp.o
	$(CC) -o client 01_client_tcp.o
server: 01_server_tcp.o
	$(CC) -o server  01_server_tcp.o
clean:
	rm -f client server *.o
