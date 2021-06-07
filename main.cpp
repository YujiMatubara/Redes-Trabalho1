#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;

int main()
{
    //criar socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Socket nao pode ser criado" << endl;
        return -1;
    }

    //ligar o socket a um ip e uma porta
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);  //comando de internet: ponteiro para um string de numero
                                                  //pega o endereco de maquina 127.0.0.1 e converte para uma string de numero
                                                    //parametro "0.0.0.0" aceita qualquer endereco

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        cerr << "Nao foi possivel ligar aa IP/port" << endl;
        return -2;
    }

    //marcar o socket para listening
    if (listen(listening, SOMAXCONN) == -1) //SOMAXCO: maximo numero de conexoes
    {
        cerr << "Erro ao criar listening." << endl;
        return -3;
    }

    //accept the call
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if (clientSocket == -1)
    {
        cerr << "Erro com o conexao do cliente." << endl;
        return -4;
    }

    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result)
    {
        cout << host << "conectado a" << svc << endl;
    }
    else
    {
      t  inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << "conectado a" << ntohs(client.sin_port) << endl;
    }

    char buf[4096];
    while (true)
    {
        //limpar  o buffer
        memset(buf, 0, 4096);
        //esperar pela mensagem
        int bytesRecv = recv(clientSocket, buf, 4096, 0);
        if (bytesRecv == -1)
        {
            cerr << "erro de conexao" << endl;
            break;
        }
        else if (bytesRecv == 0)
        {
            cout << "cliente desconectou" << endl;
            break;
        }
        
        cout << "Recebido: " << string(buf, 0, bytesRecv) << endl;  //mostrar mensagem

        send(clientSocket, buf, bytesRecv + 1, 0);  //enviar mensagem        
    }

    close(clientSocket);
    return 0;
}