make all: client server

client: gameLogicClient.o socketClient.o
	g++ -Wall -pthread gameLogicClient.o socketClient.o -o client

server: gameLogicServer.o socketServer.o
	g++ -Wall -static -pthread -lpthread gameLogicServer.o socketServer.o -o server

gameLogicClient.o: Cliente/gameLogicClient.cpp Cliente/gameLogicClient.hpp Cliente/socketClient.hpp
	g++ -c Cliente/gameLogicClient.cpp

socketClient.o: Cliente/socketClient.cpp Cliente/socketClient.hpp
	g++ -c Cliente/socketClient.cpp

gameLogicServer.o: Servidor/gameLogicServer.cpp Servidor/gameLogicServer.hpp Servidor/socketServer.hpp
	g++ -c Servidor/gameLogicServer.cpp

socketServer.o: Servidor/socketServer.cpp Servidor/socketServer.hpp
	g++ -c Servidor/socketServer.cpp

clean:
	rm -rf *.o
	rm -rf Servidor/*.gch Cliente/*.gch