#include "socketServer.hpp"
#include "gameLogicServer.hpp"

#define MSG_SIZE 256

Server::~Server() {
    // Destruir a instância de game
    delete game;
} 

//cria um map com o preGameStart e a rodada
void Server::changeGamePhase(std::string phase){
    gamePhase = phase; 
    serverPhasesFunct = serverPhases[gamePhase];
    gameCyclesFunct = gameCycles[gamePhase];
}

//comeco do jogo, mostra jogadores se conectando
void Server::preGameStart(int socket,std::string sentMessage){
    for (auto it = activePlayers.begin(); it != activePlayers.end(); it++) {
        if (it->second == 0) {
            activePlayers.erase(it);
            break;
        }
    }
    // activePlayers.erase(activePlayers.find(0)); //apaga o socket do servidor
    if(activePlayers.size()>1 && sentMessage.compare("start_game") == 0) {  //jogo soh comeca se tiver mais de 2 players
        gameRuning = true;
        endGame = false;
        gamePlayers = activePlayers.size();


        // Comecar o jogo
        game = new Game(activePlayers);// Precisa da lista de sockets para a instância ser iniciada
        //mandar msg pra todo mundo foda foda foda
        //mudar function pointer pra jogo comecado
        changeGamePhase("onGame");
        // para cada socket manda o aviso
        // std::string message = "Alow mano broder, o jogo vai começar";
        for(auto socketPair : activePlayers) {  //roda pelos sockets de cada jogador
            printf("Active player socketPair pair: %d - %d\n", socketPair.first, socketPair.second);    //mostra o id do socket e o socket do jogador
            std::string message = "playerSocket#" + std::to_string(socketPair.second) + "|playerID#" + std::to_string(socketPair.first)+"|nbPlayers#"+std::to_string(curClientsNo)+"|";    //cria a mensagem do id do jogador e o numero de players conectados
            write(socketPair.second, message.c_str(), message.size());
            printf("Mensagem de início enviada\n");
        }
    }
    else {
        std::cout << "Erro foda >>" << sentMessage << "<<\n";
    }

    return;
}

//apos conectados os jogadores inicia a rodada
void Server::onGame(int socket, std::string sentMessage) {
    std::cout << "Chamou com " << sentMessage << std::endl;
    
    if (sentMessage == "start_game") return;

    mtx.lock();
    std::unordered_map<int,std::string> toSendMessages = ((*game).*(gameActions[sentMessage]))(socket); //cria um map com da rodada e o decorrer dela
    bool weHaveAWinner =  (toSendMessages[socket].find("endGame#") != std::string::npos);
    endGame = true;
    std::string message;
    int curSocket;
    for(auto toSendMessage : toSendMessages){   //cria no map o par de socket e mensagem enviados
        message = toSendMessage.second;
        curSocket = toSendMessage.first;
        write(curSocket, message.c_str(), message.size());
    }

    std::string toSend = "Bobao";   //teste
    write(socket, toSend.c_str(), toSend.size());
    mtx.unlock();
}

void Server::playerLogMessage(int socket,int curThreadId){
    std::string message = "stdinWrite#Bem vindo ao jogo!\n|stdinWrite#Escreva algo só para testar a conexão...\n";

    write(socket, message.c_str(), message.size());

    message = "playerID#"+std::to_string(curThreadId)+"|nbPlayers#"+std::to_string(curClientsNo);

    write(socket, message.c_str(), message.size());

}

void Server::gameStartMessage(){
    std::string message = "nbPlayers#" + std::to_string(curClientsNo) + "|stdinWrite#Escreva algo só para testar a conexão...\n";
}

//Funcao remove todas as threads criadas durante a execucao do jogo
void Server::deleteOldThreads(){
    long toDelId;
    while(!toDelThreads.empty()){   //enquanto as threads nao acabarem: retira a thread da pilha e libera
        
        toDelId = toDelThreads.back();
        std::cout << "on it " << toDelId << "\n";
        threads[toDelId].join(); 
        std::cout << "Foi top\n";
        threads.erase(toDelId);
       
        activePlayers.erase(toDelId);
        toDelThreads.pop_back();
    }
}

//funcao que recebe a mensagem do cliente
bool Server::continueThread(char *msgClient,int *readSize,int socket){
    memset(msgClient, 0, strlen(msgClient) * sizeof(char)); //zera a mensagem para colocar conteudo
    if(endGame){
        *readSize = 0;
        return false;
    }
    if((*readSize = recv(socket, msgClient, MSG_SIZE , 0)) > 0 ){   //recebe a mensagem
        if(strcmp(msgClient,"quit") == 0){  //caso a mensagem esteja vazia
            if(gameRuning) endGame = true;
            *readSize = 0;
            return false;   //erro no conteudo da mensagem
        }
        return true;    //conseguiu receber a mensagem
    }
    return false;   //erro na funcao de receber mensagem
}

void Server::treatMessages(char *msgClient,int readSize,int socket){
    std::cout << "Msg nova\n";
    (*this.*serverPhasesFunct)(socket,std::string(msgClient));

}

//caso um jogador saia
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
    toDelThreads.push_back(curThreadId);    //retira a thread da pilha
    std::cout << "Thread(" << curThreadId << ") ended\n";
}

//trata as mensagens recebidas dos clientes
void* Server::connectionHandler(int curThreadId) {
    int socket = activePlayers[curThreadId];
    int readSize;
    char msgClient[MSG_SIZE];

    // playerLogMessage(socket,curThreadId);     
    
    while(continueThread(msgClient,&readSize,socket)) treatMessages(msgClient,readSize,socket); //enquanto a conexao com o cliente ocorrer, recebe as mensagens

    endingThread(curThreadId,readSize,socket);  //caso um jogador saia finaliza a thread

    return 0;
}

//configurando as variaveis do socket do servidor
void Server::setAddr(){
    addr.sin_family = AF_INET;  //familia de protocolos
    addr.sin_port = htons(serverPort);  //porta
    addr.sin_addr.s_addr =  INADDR_ANY; //ip
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));   //variaveis de 0 para o restante do tamanho
}

bool Server::validateValue(int value,std::string errorStr){
    if(value < 0){
        std::cout << errorStr << std::endl;
        return false;
    }
    return true;
}

int Server::setServerSocket() {
    socketServer = socket(AF_INET, SOCK_STREAM, 0); //cria o socket com tcp

    if(!validateValue(socketServer,"Erro ao criar o socket do servidor")) return 1; //retorna 1 nos casos de erro autoexplicativos

    setAddr();

    if(!validateValue(bind(socketServer, (struct sockaddr *) &addr, sizeof(addr)),"Erro na funcao bind()")) return 1;

    if(!validateValue(listen(socketServer,maxPlayers),"Erro na funcao listen()")) return 1;

    return 0;
}

void Server::createThread(int curThreadId){
    threads[curThreadId] = std::thread(&Server::connectionHandler,this,curThreadId);
}

//quando um jogador se conecta coloca salva a sua thread
void Server::acceptPlayer(int curThreadId){
    activePlayers[curThreadId] = accept(socketServer, NULL, NULL);
    printf("Aceito player da thread %d -> activePlayers[] = %d\n", curThreadId, activePlayers[curThreadId]);
}

void Server::awaitPlayersConnection(){
    printf("Aguardando jogador %d...\n", curClientsNo);

    acceptPlayer(threadId); //novo jogador conectado

    deleteOldThreads(); //apaga as que nao serao utiliadas
    
    if(!validateValue(activePlayers[threadId],"Erro na funcao accept()")) return;

    std::cout << "Cliente conectado (socket fd = " << activePlayers[threadId] << ")!\n";
            
    createThread(threadId); //cria thread para o jogador

    threadId++;
    curClientsNo++;
    /*
    while(gamePhase.compare("onGame") == 0){
        deleteOldThreads();
    }*/
    printf("Número de clientes = %d\n", curClientsNo);
}

void Server::afterPlayersConnection(){
    deleteOldThreads(); //apaga as que nao serao utiliadas
    while((int)toDelThreads.size() != gamePlayers){}
    mtx.lock();
    deleteOldThreads(); //apaga as que nao serao utiliadas
    delete game;
    gameRuning = false;
    endGame = false;
    changeGamePhase("preGameStart");
    mtx.unlock();
}


int Server::gameCycle() {
    while (true){
        (*this.*gameCyclesFunct)();
    }

    return 0;
}

void Server::closeServer(player * activePlayers) {
    for (int i = 0; i < maxPlayers; i++)
        close(activePlayers[i].socket); //fechando os sockets
    close(socketServer);

    return;
}

void Server::initializeServer(int maxPlayers,int serverPort){
    this->maxPlayers = maxPlayers;  //maximo players permitido pelo jogo
    this->serverPort = serverPort;
    changeGamePhase("preGameStart");  
    endGame = false;
    gameRuning = false;
    curClientsNo = 0;
    threadId = 0;
}

Server::Server(){
    Server(6, 18120);   //num players e porta definidos
}

//funcao que cria o servidor, e conecta os jogadores
Server::Server(int maxPlayers,int serverPort){
    printf("Testando socket do servidor...\n");
    initializeServer(maxPlayers,serverPort);
    setServerSocket();
    gameCycle();
    close(socketServer);
}

//para compilar:
//g++ -Wall -std=c++11 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -o testServerSocket socketServer.cpp gameLogicServer.cpp
//g++ -Wall -std=c++11 -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -o testClientSocket socketClient.cpp gameLogicClient.cpp
int main() {
    Server s;

    return 0;
}

// g++ -Wall -static -pthread -lpthread -o server socketServer.cpp gameLogicServer.cpp