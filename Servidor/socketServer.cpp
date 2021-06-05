#include "socketServer.h"

int socketServer;
struct sockaddr_in addr; // detalhes do endereço no qual o socket dará bind()
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
 int nbClients = 0; // precisará de um mutex para isso

// ================ Funções relacionadas a conexão / ao socket ================
// Gerencia a conexão com cada cliente (para testes)
void * connectionHandler(void * socketDesc) {
    int socket = *(int*) socketDesc; // socket descriptor
    int readSize;
    std::string message;
    char msgClient[MSG_SIZE];
    
    message = "Bem-vindo ao jogo!\n";
    write(socket, message.c_str(), message.length());

    message = "Escreva algo só para testar a conexão...\n";
    write(socket, message.c_str(), message.length());
     
    // Recebe a mensagem do cliente
    while( (readSize = recv(socket, msgClient, MSG_SIZE , 0)) > 0 ) {
		msgClient[readSize] = '\0'; // marcar o final da string
		
		// Manda a mensagem de volta para o cliente
        write(socket, msgClient, strlen(msgClient));
		
		// Limpa o buffer de mensagem
		memset(msgClient, 0, MSG_SIZE);
    }
     
    if(readSize == 0) {
        printf("Cliente desconectado (socket nb %d\n", socket);
        close(socket);
        nbClients--;
        printf("Número de clientes = %d\n", nbClients);
        fflush(stdout);
        pthread_exit(NULL);
    }
    else if(readSize == -1) {
        printf("Erro no recv()");
        nbClients--;
        pthread_exit(NULL);
    }
         
    return 0;
}

// Função que gerencia o envio de mensagens do servidor para o currPlayer (um dos clientes)
void sendMsg(player * currPlayer, char * msg) {
    int sent;
    
    msg[MSG_SIZE] = '\0'; // evitando leitura de memória indevida...
    sent = send(currPlayer->socket, msg, strlen(msg), 0);

    if (sent == -1)
        std::cout << "Erro ao enviar mensagem\n";
    else
        std::cout << "Mensagem enviada com sucesso\n";

    return;
}

// Função que gerencia o recebimento de mensagens do currPlayer para o servidor
void listener(player * currPlayer){
    int recieved;
    char answer[MSG_SIZE];
    do {
        recieved = recv(currPlayer->socket, answer, MSG_SIZE, 0); // recebe mensagem do cliente (MSG_SIZE bytes)
        if (recieved == -1)
            std::cout << "Erro ao receber mensagem do socket \n" << currPlayer->socket << std::endl;
        else {
            answer[recieved] = '\0';
            printf("Mensagem do cliente recebida: %s\n", answer);
        }
    } while(strcmp(answer, "exit") != 0); 
    // pthread_mutex_destroy(&mutexsum); // mutex
    // pthread_exit(NULL);
    // done = 0; // flag da conexão

    return;
}

// Funçã que seta o socket do servidor e deixa ele pronto para esperar conexões (escutando em uma porta)
int setServerSocket() {
    socketServer = socket(AF_INET, SOCK_STREAM, 0); // retorna um FD (file descriptor) para o socket
    // AF_INET = família de protocolos selecionada
    // SOCK_STREAM = TCP/IP e SOCK_DGRAM = UDP/IP
    // 0 indica uso do protocolo padrão

    if(socketServer == -1) {
        std::cout << "Erro ao criar o socket do servidor\n";
        return 1; // != 0 significa erro
    }

    addr.sin_family = AF_INET; // familía de protocolos
    addr.sin_port = htons(1234); // porta onde o serviço será exposto
    addr.sin_addr.s_addr = INADDR_ANY; // enderereço IP do host
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    if(bind(socketServer, (struct sockaddr *) &addr, sizeof(addr)) == -1) { // bind a uma porta -> em que porta o processo está
        std::cout << "Erro na funcao bind()\n";
        return 1; // != 0 significa erro
    }

    if(listen(socketServer, 5) == -1) { // escuta na porta no qual o socket deu bind() e espera por conexões
        std::cout << "Erro na funcao listen()\n";
        return 1; // != 0 significa erro
    }

    return socketServer; // tudo ok, pode esperar clientes
}

// Função que espera pela conexão dos NB_PLAYERS jogadores
int awaitPlayersConnection(player * activePlayers) {
    pthread_t threadId[NB_PLAYERS];

    for (int i = 0; i < NB_PLAYERS; i++) {
        printf("Aguardando jogador %d...\n", i);

        activePlayers[i].socket = accept(socketServer, 0, 0); // aceita conexão no socketServer
        // activePlayers[i].socket -> armazena o file descriptor da conexão criada

        if(activePlayers[i].socket == -1) {
            std::cout << "Erro na funcao accept()\n";
            return 1;
        }

        std::cout << "Cliente conectado (socket fd = " << activePlayers[i].socket << ")!\n";
        nbClients++;
        
        // MUDAR AQUI ---> PARA TESTAR CRIEI UMA THREAD SÓ PARA CADA CLIENTE, MAS 
        // O IDEAL É SEGUIR A IDEIA QUE ESTÁ AQUI EM BAIXO
        // DANDO 'JOIN' EM DUAS THREADS. UMA ESCUTA E OUTRA MANDA MENSAGEM
        // É PRECISO MODIFICAR AS FUNÇÕES DE ENVIAR E RECEBER MENSAGEM
        // PARA ADEQUAR AO USO DE THREADS 

        // pthread_t threads[2];
        // pthread_attr_t attr;

        // pthread_attr_init(&attr);
        // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        // pthread_create(&threads[0], &attr, sendMsg, NULL);
        // pthread_create(&threads[1], &attr, listener, NULL);

        if( pthread_create( &threadId[i] , NULL ,  connectionHandler, (void*) &(activePlayers[i].socket)) != 0)
        {
            printf("Não foi possível criar thread corretamente!\n");
            return 1;
        }

        printf("Número de clientes = %d\n", nbClients);
    }

    while (nbClients > 0) { continue; } // aqui é possível fazer o servidor ficar cuidando do que acontece talvez (?)

    return 0;
}

void closeServer(player * activePlayers) {
    for (int i = 0; i < NB_PLAYERS; i++)
        close(activePlayers[i].socket);
    close(socketServer);

    return;
}

// Isso é só para teste, apagar depois
// g++ -Wall -pthread -o testServerSocket socketServer.cpp
int main() {
    player activePlayers[NB_PLAYERS];
    printf("Testando socket do servidor...\n");
    int socketServer = setServerSocket();
    awaitPlayersConnection(activePlayers);
    
    close(socketServer);

    return 0;
}