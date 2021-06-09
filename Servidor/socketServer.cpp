#include "socketServer.hpp"
#include "gameLogicServer.hpp"

#define MSG_SIZE 256

Server::~Server() {
    // Destrói a instância de game, que é o único atributo que pode ser destruído
    delete game;
} 

// Atualiza a fase do jogo
void Server::changeGamePhase(std::string phase){
    gamePhase = phase; 
    serverPhasesFunct = serverPhases[gamePhase];
    gameCyclesFunct = gameCycles[gamePhase];
}

// Faz a organização inicial do jogo, e mostra jogadores se conectando
void Server::preGameStart(int socket,std::string sentMessage){
    // Remove jogadores inativos
    for (auto it = activePlayers.begin(); it != activePlayers.end(); it++) {
        if (it->second == 0) {
            activePlayers.erase(it);
            break;
        }
    }

    // Se algum jogador solicitou o início da partida (precisa de pelo menos 2 clientes ativos)
    if(activePlayers.size()>1 && sentMessage.compare("start_game") == 0) {
        gameRuning = true; 
        endGame = false;
        gamePlayers = activePlayers.size();

        // Instancia o objeto jogo passando os sockets dos jogadores ativos para o construtor
        game = new Game(activePlayers);

        // Muda a function pointer pra jogo comecado
        changeGamePhase("onGame");
        
        // Avisa todos os jogadores conectados pelo socket que o jogo começou
        for(auto socketPair : activePlayers) {
            // Registra no servidor qual jogador receberá a mensagem agora
            printf("Jogador ativo: %d\n socket do jogador: %d\n", socketPair.first, socketPair.second);
            // Monta a mensagem que será passada
            std::string message = "playerSocket#" + std::to_string(socketPair.second) + "|playerID#" + std::to_string(socketPair.first)+"|nbPlayers#"+std::to_string(curClientsNo)+"|";
            write(socketPair.second, message.c_str(), message.size());
            printf("Mensagem de início de jogo enviada com sucesso!\n");
        }
    }
    else {
        // Repassa o erro ao servidor
        std::cout << "ERRO: a partida não possui pelo menos 2 jogadores ou a mensagem está incorreta (é " << sentMessage << " e deveria ser start_game)\n";
    }

    return;
}

// Com todos os jogadoes conectados, a partida é iniciada
void Server::onGame(int socket, std::string sentMessage) {
    std::cout << "Solicitação recebida: " << sentMessage << std::endl;
    
    // Não pode ser a mensagem start_game, pois ela é usada em outra função
    if (sentMessage == "start_game") return;

    // Todo o trecho abaixo precisa ser realizado com o mutex em lock
    mtx.lock();
    // Cria um map com da rodada e o decorrer dela
    std::unordered_map<int,std::string> toSendMessages = ((*game).*(gameActions[sentMessage]))(socket);
    
    // Se alguém ficou sem cartas, precisa atualizar o estado do jogo para os jogadores
    bool weHaveAWinner =  (toSendMessages[socket].find("endGame#") != std::string::npos);
    if (weHaveAWinner) endGame = true;

    // Escreve a mensagem do map para cada socket (avisa todos os jogadores)
    std::string message;
    int curSocket;
    for(auto toSendMessage : toSendMessages){
        message = toSendMessage.second;
        curSocket = toSendMessage.first;
        write(curSocket, message.c_str(), message.size());
    }

    std::cout << "Mensagens enviadas para todos os jogadores conectados!\n";
    mtx.unlock();
}

// Função que escreve a mensagem de boas vindas ao jogador recém conectado
void Server::playerLogMessage(int socket,int curThreadId){
    //Registra o texto a ser passado por socket
    std::string message = "stdinWrite#Bem vindo ao jogo!\n|stdinWrite#Escreva algo só para testar a conexão...\n";

    // Envia a mensagem para o jogador específico
    write(socket, message.c_str(), message.size());

    // Prepara e envia ao novo jogador uma mensagem com o seu próprio ID e a quantidade de jogadores
    message = "playerID#"+std::to_string(curThreadId)+"|nbPlayers#"+std::to_string(curClientsNo);
    write(socket, message.c_str(), message.size());

    std::cout << "Boas vindas enviadas com sucesso para o novo jogador!\n";
}

//Funcao remove todas as threads criadas durante a execucao do jogo
void Server::deleteOldThreads(){
    long toDelId;
    while(!toDelThreads.empty()){   //enquanto as threads nao acabarem: retira a thread da pilha e libera
        
        toDelId = toDelThreads.back();
        std::cout << "Deletando a thread de id " << toDelId << "\n";
        threads[toDelId].join(); 
        threads.erase(toDelId);
       
        activePlayers.erase(toDelId);
        toDelThreads.pop_back();
        std::cout << "A thread antiga foi removida com sucesso!\n";
    }
}

//funcao que recebe a mensagem do cliente
bool Server::continueThread(char *msgClient,int *readSize,int socket){
    memset(msgClient, 0, strlen(msgClient) * sizeof(char)); //zera a mensagem para colocar o conteudo
    if(endGame){
        // A partida já acabou, a thread não vai continuar e não precisa receber a mensagem
        *readSize = 0;
        return false;
    }
    if((*readSize = recv(socket, msgClient, MSG_SIZE , 0)) > 0 ){   //recebe a mensagem
        if(strcmp(msgClient,"quit") == 0){  // Caso seja a mensagem para sair do jogo
            // Marca que o jogo acabou e informa que a thread não vai continuar
            if(gameRuning) endGame = true;
            *readSize = 0;
            return false;   //erro no conteudo da mensagem
        }
        // Não é a mensagem de fim de jogo, então a thread vai continuar
        return true;
    }
    return false;   //erro na funcao de receber mensagem
}

// Utiliza um ponteiro de função para chamar qualquer função da classe por meio dos argumentos
void Server::treatMessages(char *msgClient,int readSize,int socket){
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
        // Finaliza a thread
    }
    else if(readSize == -1) {
        // Não recebeu a mensagem do socket
        close(socket);
        printf("Erro no recv()");
        // Finaliza a thread
    }
    toDelThreads.push_back(curThreadId);    //retira a thread da pilha
    std::cout << "Thread(" << curThreadId << ") foi encerrada\n";
}

//trata as mensagens recebidas dos clientes
void* Server::connectionHandler(int curThreadId) {
    int socket = activePlayers[curThreadId];
    int readSize;
    char msgClient[MSG_SIZE];
    
    //enquanto a conexao com o cliente ocorrer, recebe as mensagens
    while(continueThread(msgClient,&readSize,socket)) treatMessages(msgClient,readSize,socket); 

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

// Verifica se é uma string de erro ou mensagem
bool Server::validateValue(int value,std::string errorStr){
    if(value < 0){
        //Erro, informa qual é o problema no console
        std::cout << "ERRO: " << errorStr << std::endl;
        return false;
    }
    return true;
}

// Cria um socket para um jogador
int Server::setServerSocket() {
    //cria o socket com tcp
    socketServer = socket(AF_INET, SOCK_STREAM, 0);

    // Retorna 1 nos casos de erro autoexplicativos
    if(!validateValue(socketServer,"Erro ao criar o socket do servidor")) return 1;

    setAddr();

    // Tenta executar a função bind
    if(!validateValue(bind(socketServer, (struct sockaddr *) &addr, sizeof(addr)),"Erro na funcao bind()")) return 1;

    // Tenta executar a função listen
    if(!validateValue(listen(socketServer,maxPlayers),"Erro na funcao listen()")) return 1;

    std::cout << "Socket configurado com sucesso!\n";
    return 0;
}

// Função que cria uma nova thread
void Server::createThread(int curThreadId){
    threads[curThreadId] = std::thread(&Server::connectionHandler,this,curThreadId);
}

//quando um jogador se conecta, salva a sua thread
void Server::acceptPlayer(int curThreadId){
    activePlayers[curThreadId] = accept(socketServer, NULL, NULL);
}

// Função que fica aguardando os jogadores para o início da partida
void Server::awaitPlayersConnection(){
    printf("Aguardando jogador %d...\n", curClientsNo);

    acceptPlayer(threadId); //novo jogador conectado

    deleteOldThreads(); //apaga as threads que não serão utiliadas
    
    if(!validateValue(activePlayers[threadId],"Erro na funcao accept()")) return;

    std::cout << "Cliente conectado (socket fd = " << activePlayers[threadId] << ")!\n";
            
    createThread(threadId); //cria thread para o jogador

    threadId++;
    curClientsNo++;

    printf("Número de clientes = %d\n", curClientsNo);
}

// Organiza a partida assim que todos os players estão conectados
void Server::afterPlayersConnection(){
    deleteOldThreads(); //apaga as threads que nao serao utiliadas
    
    while((int)toDelThreads.size() != gamePlayers){} //espera o jogo acabar - isso é sinalizado quando as threads a deletar são iguais ao número de jogadores
    
    mtx.lock();
    deleteOldThreads(); //apaga as que nao serao utiliadas
    delete game;
    gameRuning = false;
    endGame = false;
    changeGamePhase("preGameStart");
    mtx.unlock();
}

// Representa os ciclos do jogo, fica rodando inteterminadamente
int Server::gameCycle() {
    while (true){
        (*this.*gameCyclesFunct)();
    }

    return 0;
}

// Desliga o servidor
void Server::closeServer() {
    for (auto & players : activePlayers) close(players.second);

    std::cout << "O servidor será desligado\n";    
    close(socketServer);

    return;
}

// Inicializa o servidor
void Server::initializeServer(int maxPlayers,int serverPort){
    this->maxPlayers = maxPlayers;  //maximo players permitido pelo jogo
    this->serverPort = serverPort;
    changeGamePhase("preGameStart"); // Informa a fase atual do jogo 
    endGame = false;
    gameRuning = false;
    curClientsNo = 0;
    threadId = 0;
    std::cout << "Servidor inicializado\n";
}

// Construtor do servidor que não utiliza parâmetros
Server::Server(){
    Server(6, 18120);   //num players e porta definidos
}

//funcao que cria o servidor, e conecta os jogadores
Server::Server(int maxPlayers,int serverPort){
    initializeServer(maxPlayers,serverPort);
    setServerSocket();
    gameCycle();
    close(socketServer);
}

Server server;

// Função que elimina o funcionamento do jogo
void terminateAll(int signum) {
    server.closeServer();
    exit(signum);
}

// Trabalha com sinais para decidir se o jogo vai ser encerrado
void handleSignals(){
    std::vector<int> handledSignals = {SIGTERM,SIGSEGV,SIGINT,SIGILL,SIGABRT,SIGFPE};
    for(auto _signal : handledSignals) signal(_signal,terminateAll);
}

// Função main, usada para a manipulação de sinais
int main() {
    handleSignals();
    return 0;
}