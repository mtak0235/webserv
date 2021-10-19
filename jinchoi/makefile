
SERVER_OBJS = ServerSocket.o Socket.o main_server.o
CLI_OBJS = ClientSocket.o Socket.o main_client.o

all: server client

server: $(SERVER_OBJS)
	g++ -o test_server $(SERVER_OBJS)

client: $(CLI_OBJS)
	g++ -o test_cli $(CLI_OBJS)

clean:
	rm -f *.o test_server test_cli
