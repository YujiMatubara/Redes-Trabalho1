#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <string>
#include <pthread.h>
#include <vector>

int meu_socket;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
struct sockaddr_in addr;
void *sendmessage(void *);
void *listener(void *);
int done = 1;

int main()
{

    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (meu_socket == -1)
    {
        printf("Erro ao criar o socket!\n");
        return 1;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(18120);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));

    printf("Tentando se conectar ao servidor...\n");

    if (connect(meu_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        printf("Erro ao se conectar!\n");
        return 1;
    }

    printf("Conectado!\n\n");

    pthread_t threads[2];
    void *status;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_create(&threads[0], &attr, sendmessage, NULL);
    pthread_create(&threads[1], &attr, listener, NULL);

    while (done)
    {
    }

    return 0;
}

void *sendmessage(void *)
{
    int enviados;
    char mensagem[256];

    do
    {
        printf("Cliente: ");
        fgets(mensagem, 256, stdin);
        mensagem[strlen(mensagem) - 1] = '\0';
        enviados = send(meu_socket, mensagem, strlen(mensagem), 0);

    } while (strcmp(mensagem, "exit") != 0);

    pthread_mutex_destroy(&lock);
    pthread_exit(NULL);
    close(meu_socket);
    done = 0;
}

void *listener(void *)
{
    int recebidos;
    char resposta[256];
    do
    {
        recebidos = recv(meu_socket, resposta, 256, 0);
        resposta[recebidos] = '\0';
        printf("\n Servidor: %s\n", resposta);

    } while (recebidos != -1);
}