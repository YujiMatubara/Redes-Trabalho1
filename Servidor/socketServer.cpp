#include "socketServer.hpp"


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


std::string Server::preGameStart(){
   return "tamoBemzao"; 
};
std::string Server::onGame(){
    return "tamoMalzinho";
};



void Server::treatMessages(char *msgClient,int readSize,int socket){
    mtx.lock();

    std::cout << "Foi\n";
    msgClient[readSize] = '\0'; 
    std::string toWrite =  ((*this).*(serverPhases[gamePhase))();
    write(socket, toWrite.c_str(), strlen(toWrite.c_str()));
    memset(msgClient, 0, MSG_SIZE);
    mtx.unlock();
}

void Server::endingThread(int curThreadId,int readSize,int socket){
    mtx.lock();
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
    mtx.unlock();
    std::cout << "Thread(" << curThreadId << ") ended\n";     
}

int Server::msgSize(int socket){
    char msgClient[MSG_SIZE];
    int readSize = recv(socket, msgClient, MSG_SIZE , 0);
    return  (msgClient[0] == 'q'? 0 : readSize);
}


void* Server::connectionHandler(int curThreadId) {
    int socket = activePlayers[curThreadId].socket;
    int readSize;
    std::cout << "Here?2\n";
    int readSize;
    std::string message;

    message = "Bem-vindo ao jogo!\n";
    write(socket, message.c_str(), message.length());

    message = "Escreva algo só para testar a conexão...\n";
    write(socket, message.c_str(), message.length());
    
    while((readSize =  msgSize(socket)) > 0 ) treatMessages(msgClient,readSize,socket);

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
    activePlayers[curThreadId].socket = accept(socketServer, NULL, NULL); 
}

int Server::awaitPlayersConnection() {
    while (curClientsNo < maxPlayers){
        printf("Aguardando jogador %d...\n", curClientsNo);

        acceptPlayer(threadId);

        mtx.lock();

        deleteOldThreads();
        
        if(!validateValue(activePlayers[threadId].socket,"Erro na funcao accept()")) return 1;

        std::cout << "Cliente conectado (socket fd = " << activePlayers[threadId].socket << ")!\n";
                
        createThread(threadId);

        printf("Número de clientes = %d\n", curClientsNo);

        threadId++;
        curClientsNo++;

        mtx.unlock();
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
    gamePhase = "preGameStart";

    startGame = true; 
   
    curClientsNo = 0;
    threadId = 0;
}

Server::Server(){
    Server(6,18120);
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