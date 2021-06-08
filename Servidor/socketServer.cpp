#include "socketServer.hpp"
#include "gameLogicServer.hpp"

#define MSG_SIZE 256

void Server::changeGamePhase(std::string phase){
    gamePhase = phase; 
    desiredFunct = serverPhases[gamePhase];
}


void Server::preGameStart(int socket,std::string sentMessage){
    std::cout <<"Numm = " << activePlayers.size() << std::endl;
    if(activePlayers.size()>1 && sentMessage.compare("game_start") == 0) {
        std::cout << "Eh pra comecar\n";
        // Comecar o jogo
        game = new Game(activePlayers);// Precisa da lista de sockets para a instância ser iniciada
        //mandar msg pra todo mundo foda foda foda
        //mudar function pointer pra jogo comecado
        changeGamePhase("onGame");
        // para cada socket manda o aviso
        // std::string message = "Alow mano broder, o jogo vai começar";
        for(auto socket : activePlayers) {
            if (socket.second == 0) continue; // aqui tem algum problema, resolver e remover esse if! -> activePlayersNB está com um jogador a mais
            printf("Active player socket pair: %d - %d\n", socket.first, socket.second);
            std::string message = "playerId#" + std::to_string(socket.second)+"|nbPlayers#"+std::to_string(curClientsNo)+"|";
            write(socket.second, message.c_str(), message.size());
            // printf("Mensagem de início enviada\n");
        }
    }
    else {
        std::cout << "Erro foda >>" << sentMessage << "<<\n";
    }

    std::string toSend = "Bobinho";
    write(socket, toSend.c_str(), toSend.size());

    return; 
}

void Server::onGame(int socket,std::string sentMessage){
    std::unordered_map<int,std::string> toSendMessages = ((*game).*(gameActions[sentMessage]))(socket);
    
    std::string message;
    int curSocket;
    for(auto toSendMessage : toSendMessages){
        printf("AQUI\n");
        message = toSendMessage.second;
        curSocket = toSendMessage.first;
        write(curSocket, message.c_str(), message.size());
    }

    std::string toSend = "Bobao";
    write(socket, toSend.c_str(), toSend.size());

}

void Server::playerLogMessage(int socket,int curThreadId){
    std::string message = "stdinWrite#Bem vindo ao jogo!\n|stdinWrite#Escreva algo só para testar a conexão...\n";

    write(socket, message.c_str(), message.size());

    message = "playerId#"+std::to_string(curThreadId)+"|nbPlayers#"+std::to_string(curClientsNo);

    write(socket, message.c_str(), message.size());

}

void Server::gameStartMessage(){

    std::string message = "nbPlayers#" + std::to_string(curClientsNo) + "|stdinWrite#Escreva algo só para testar a conexão...\n";

}

void Server::deleteOldThreads(){
    long toDelId;
    while(!toDelThreads.empty()){
        
        toDelId = toDelThreads.back();
        std::cout << "on it " << toDelId << "\n";
        threads[toDelId].join();
        std::cout << "Foi top\n";
        threads.erase(toDelId);
       
        activePlayers.erase(toDelId);
        toDelThreads.pop_back();
    }
}

bool Server::continueThread(char *msgClient,int *readSize,int socket){
    if((*readSize = recv(socket, msgClient, MSG_SIZE , 0)) > 0 ){
        if(strcmp(msgClient,"quit") == 0){
            *readSize = 0;
            return false;
        }
        return true;
    }
    return false;
}

void Server::treatMessages(char *msgClient,int readSize,int socket){
    std::cout << "Msg nova\n";
    (*this.*desiredFunct)(socket,std::string(msgClient));
}

void Server::endingThread(int curThreadId,int readSize,int socket){
    curClientsNo--;
    if(readSize == 0) {
        printf("Cliente desconectado (socket nb %d\n", socket);
        close(socket);
        printf("Número de clientes = %d\n", curClientsNo);
        fflush(stdout);
        //matar thread
    }
    else if(readSize == -1) {
        close(socket);
        printf("Erro no recv()");
        //matar thread
    }
    toDelThreads.push_back(curThreadId);
    std::cout << "Thread(" << curThreadId << ") ended\n";     
}

void* Server::connectionHandler(int curThreadId) {
    int socket = activePlayers[curThreadId];
    int readSize;
    char msgClient[MSG_SIZE];

    // playerLogMessage(socket,curThreadId);     
    

    while(continueThread(msgClient,&readSize,socket)) treatMessages(msgClient,readSize,socket);

    endingThread(curThreadId,readSize,socket);

    return 0;
}

void Server::setAddr(){
    addr.sin_family = AF_INET;
    addr.sin_port = htons(serverPort);
    addr.sin_addr.s_addr =  INADDR_ANY;
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
}

bool Server::validateValue(int value,std::string errorStr){
    if(value < 0){
        std::cout << errorStr << std::endl;
        return false;
    }
    return true;
}

int Server::setServerSocket() {
    socketServer = socket(AF_INET, SOCK_STREAM, 0);

    if(!validateValue(socketServer,"Erro ao criar o socket do servidor")) return 1;

    setAddr();

    if(!validateValue(bind(socketServer, (struct sockaddr *) &addr, sizeof(addr)),"Erro na funcao bind()")) return 1;

    if(!validateValue(listen(socketServer,maxPlayers),"Erro na funcao listen()")) return 1;

    return 0;
}

void Server::createThread(int curThreadId){
    threads[curThreadId] = std::thread(&Server::connectionHandler,this,curThreadId);
}

void Server::acceptPlayer(int curThreadId){
    activePlayers[curThreadId] = accept(socketServer, NULL, NULL);
    printf("Aceito player da thread %d -> activePlayers[] = %d\n", curThreadId, activePlayers[curThreadId]);
}

int Server::awaitPlayersConnection() {
    while (curClientsNo < maxPlayers){
        printf("Aguardando jogador %d...\n", curClientsNo);

        acceptPlayer(threadId);


        deleteOldThreads();
        
        if(!validateValue(activePlayers[threadId],"Erro na funcao accept()")) return 1;

        std::cout << "Cliente conectado (socket fd = " << activePlayers[threadId] << ")!\n";
                
        createThread(threadId);

        threadId++;
        curClientsNo++;

        printf("Número de clientes = %d\n", curClientsNo);
    }

    while (curClientsNo > 0) { continue; } 

    return 0;
}

void Server::closeServer(player * activePlayers) {
    for (int i = 0; i < maxPlayers; i++)
        close(activePlayers[i].socket);
    close(socketServer);

    return;
}

void Server::initializeServer(int maxPlayers,int serverPort){
    this->maxPlayers = maxPlayers;
    this->serverPort = serverPort;
    changeGamePhase("preGameStart");  
   
    curClientsNo = 0;
    threadId = 0;
}

Server::Server(){
    Server(6, 18120);
}

Server::Server(int maxPlayers,int serverPort){
    printf("Testando socket do servidor...\n");
    initializeServer(maxPlayers,serverPort);
    setServerSocket();
    awaitPlayersConnection();
    close(socketServer);
}

//g++ -Wall -std=c++11 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -o testServerSocket socketServer.cpp
int main() {
    Server s;    

    return 0;
}