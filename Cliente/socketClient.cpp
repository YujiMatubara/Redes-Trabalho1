#include "socketClient.hpp"

// struct sockaddr_in{ //dados do host para comunicacao
//     short int sin_family;
//     unsigned short int sin_port;
//     struct in_addr sin_addr;
//     unsigned char sin_zero[8];
// }

/*  inicializa o socket do servidor
 *  retorno:
 *  retorna o socket caso valido
 *  retorna -1 em caso de erro
 */
int initializeSocket(){
    int socketClient;
    socketClient = socket(AF_INET, SOCK_STREAM, 0); //cri socket com: familia do servidor, utiliza o metodo TCP, protocolo IP internet
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
int startConnection(int socketClient, int port, const char ip[15])
{
    struct sockaddr_in host;
    host.sin_family = AF_INET;  //familia de protocolo: ARPA INTERNET
    host.sin_port = htons(port);   //porta do servidor
    host.sin_addr.s_addr = inet_addr(ip);   //ip do servidor
    memset(&host.sin_zero, 0, sizeof(host.sin_zero));   //preencher o sin_zero com 0
    if (connect(socketClient, (struct sockaddr*)&host, sizeof(host)) == -1) //conecta com o servidor
    {
        std::cerr << "Não foi possível conectar-se ao servidor." << std::endl;
        return -1;  //erro
    }
    return 0;
}

/*  arrumar
 */
void sendMessage(int socketClient)
{
    int sended = 0;
    char message[256];
    while (strcmp(message, "exit") != 0)
    {
        std::cout << "Escreva a mensagem que deseja enviar." << std::endl;
        fgets(message, strlen(message), stdin);
        message[strlen(message)-1] = '\0';
        sended = send(socketClient, message, strlen(message), 0);
    }
    std::cout << "Mensagem: " << sended << std::endl;

    return;
}

/*  arrumar
 */
void receivedMessage(int socketClient)
{
    int received;
    char reply[256];

    while(received != -1)
    {
        received = recv(socketClient, reply, 256, 0);
        reply[received] = '\0';
        std::cout << "Resposta: " << reply << std::endl;
    }

    return;
}

int main()
{
    int socketClient = initializeSocket();
    startConnection(socketClient, 18120, "127.0.0.1");
    sendMessage(socketClient);
}