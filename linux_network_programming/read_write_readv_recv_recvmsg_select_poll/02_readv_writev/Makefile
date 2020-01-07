CC = gcc
CLIENT=readv_writev_client
SERVER=readv_writev_server
CLIENT_TARGERT=client
SERVER_TARGERT=server

all:client server	
client:$(CLIENT).o 
	$(CC) -o $(CLIENT_TARGERT) $(CLIENT).o  -g
	
server:$(SERVER).o 
	$(CC) -o $(SERVER_TARGERT) $(SERVER).o  -g
clean:
	$(RM) *.o $(CLIENT_TARGERT) $(SERVER_TARGERT)