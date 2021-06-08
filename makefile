client: gameLogicClient.o socketClient.o
	g++  -Wall -static -pthread -lpthread gameLogicClient.o socketClient.o -o client

server: gameLogicServer.o socketServer.o
	g++ -Wall -static -pthread -lpthread gameLogicServer.o socketServer.o -o server

gameLogicClient.o:
	g++ -c Cliente/gameLogicClient.cpp Cliente/gameLogicClient.hpp

socketClient.o:
	g++ -c Cliente/socketClient.cpp Cliente/socketClient.hpp

gameLogicServer.o:
	g++ -c Servidor/gameLogicServer.cpp Servidor/gameLogicServer.hpp

socketServer.o:
	g++ -c Servidor/socketServer.cpp Servidor/socketServer.hpp

clean:
	rm -rf *.o

# // g++ -Wall -static -pthread -lpthread -o server socketServer.cpp gameLogicServer.cpp