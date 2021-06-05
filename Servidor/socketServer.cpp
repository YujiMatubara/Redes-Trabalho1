#include "socketServer.h"

int socketServer;
struct sockaddr_in addr;
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;

// ================ Funções relacionadas a conexão / ao socket ================
// Envia mensagem do servidor para o currPlayer (um dos clientes)
void * sendMsg(player * currPlayer, char * msg){
    int  sent;
    
    // talvez aqui precise de um while (?) não sei como funciona direito ptreahds

    // printf("Server: ");
    // fgets(msg, 256, stdin);
    msg[strlen(msg)-1] = '\0'; // evitando leitura de memória indevida...
    sent = send(currPlayer->socket, msg, strlen(msg), 0);

    if (sent == -1)
        std::cout << "Erro ao enviar mensagem\n";
    else
        std::cout << "Mensagem enviada com sucesso\n";

    return;
}

void * listener(player * currPlayer){
    int recieved;
    char answer[256];
    do {
        recieved = recv(currPlayer->socket, answer, 256, 0); // recebe mensagem do cliente (256 bytes)
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

int setServerSocket() {
    socketServer = socket(AF_INET,SOCK_STREAM, 0);

    if(socketServer == -1) {
        std::cout << "Erro ao criar o socket do servidor\n";
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
    addr.sin_addr.s_addr = INADDR_ANY;
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    if(bind(socketServer, (struct sockaddr*) &addr, sizeof(addr)) == -1) {
        std::cout << "Erro na funcao bind()\n";
        return 1;
    }

    if(listen(socketServer, 1) == -1) {
        std::cout << "Erro na funcao listen()\n";
        return 1;
    }

    return 0; // tudo ok, pode esperar clientes
}

int awaitPlayersConnection() {
    for (int i = 0; i < NB_PLAYERS; i++) {
        printf("Aguardando cliente...\n");

        activePlayers[i].socket = accept(socketServer, 0, 0);

        if(activePlayers[i].socket == -1) {
            std::cout << "Erro na funcao accept()\n";
            return 1;
        }

        std::cout << "Cliente conectado!\n";
        pthread_t threads[2];
        void *status;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        pthread_create(&threads[0], &attr, sendMsg, NULL); // erro aqui!
        pthread_create(&threads[1], &attr, listener, NULL); // erro aqui!
    }
}