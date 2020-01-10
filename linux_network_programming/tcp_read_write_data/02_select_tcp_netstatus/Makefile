
CC = gcc
all:client server
	
client:02_client_tcp.o
	$(CC) -o client 02_client_tcp.o
server: 02_server_tcp.o
	$(CC) -o server  02_server_tcp.o
clean:
	rm -f client server *.o
