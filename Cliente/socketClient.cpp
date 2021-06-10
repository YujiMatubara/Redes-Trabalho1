#include "socketClient.hpp"

/*  inicializa o socket do servidor
 *  retorno:
 *  retorna o socket caso valido
 *  retorna -1 em caso de erro
 */
int initializeSocket() {
    int socketClient;
    socketClient = socket(AF_INET, SOCK_STREAM, 0); //cria socket com: familia do servidor, utiliza o metodo TCP, protocolo IP internet
    if (socketClient == -1) //erro
    {
        std::cerr << "Socket nao pode ser criado" << std::endl;
        return -1;
    }

    return socketClient;
}

/*  inicializa a conexao com o servidor
 *  retorno:
 *  retorna 0 caso valido
 *  retorna -1 em caso de erro
 */
int startConnection(int socketClient, int port, const char ip[16]) {
    struct sockaddr_in host;
    host.sin_family = AF_INET;  //familia de protocolo: ARPA INTERNET
    host.sin_port = htons(port);   //porta do servidor
    if (!strcmp(ip, "localhost"))
        host.sin_addr.s_addr = inet_addr("127.0.0.1");
    else
        host.sin_addr.s_addr = inet_addr(ip);   //ip do servidor
    memset(&host.sin_zero, 0, sizeof(host.sin_zero));   //preencher o sin_zero com 0
    if (connect(socketClient, (struct sockaddr*)&host, sizeof(host)) == -1) //conecta com o servidor
    {
        std::cerr << "Não foi possível conectar-se ao servidor." << std::endl;
        return -1;  //erro
    }
    return 0;
}

/*  envio de mensagens ao servidor
 *  coloca a mensagem no parametro message
 *  em caso de erro printa
 */
void sendMessage(int socketClient, const char * message) {
    int sended = 0;
    sended = send(socketClient, message, strlen(message), 0);

    if (sended == -1)   //se a funcao send deu erro
        printf("Erro no envio da mensagem para o servidor.\n");

    return;
}

/*  recebe as mensagens do servidor
 *  coloca a mensagem em reply
 */ 
std::string receiveMessage(int socketClient) {
    int received;
    char reply[256];

    do {    //enquanto a mensagem do servidor nao acabar
        received = recv(socketClient, reply, 256, 0);
        reply[received] = '\0';
        // printf("Reply = %s\n", reply);
    } while (received <= 0);

    std::string stringReply = reply;    //retorna em string
    return stringReply;
}