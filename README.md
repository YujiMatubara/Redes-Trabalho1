# Redes - Trabalho 1
Trabalho para a disciplina SSC0142 - Redes de Computadores

# Grupo
    Gabriel Monteiro Ferracioli
    Lucas Yuji Matubara
    Pedro Guerra Lourenço
    Vinicius Eduardo de Araujo

# Como executar:

Os executáveis foram testados em sistemas Linux Ubuntu, nas versões `20.04.2 LTS` e `18.04.5 LTS`.

Para compilar os arquivos do servidor e do cliente, dentro da pasta do projeto dê um `make` e dois executáveis serão compilados: `client` e `server`. O `client` refere-se ao cliente que vai jogar o jogo e o `server` refere-se ao servidor que hosperadá o jogo.

Em vez de executar o Makefile, é possível compilar diretamente com o `gcc` com:
- Cliente: `g++ -Wall -o client -lpthread gameLogicClient.cpp socketClient.cpp`
- Servidor: `g++ -Wall -static -pthread -lpthread -o server socketServer.cpp gameLogicServer.cpp`

Para executar o cliente, basta dar o comando `./client <IP_DO_SERVIDOR> <PORTA_DO_SERVIDOR>` dentro da pasta `Cliente`, sendo o servidor o endereço na qual você deseja conectar-se.

Já para executar o servidor, basta dar o comando `./server` dentro da pasta `Servidor`.
    
# Breve descrição do trabalho:
Nesse trabalho, foi desenvolvido uma versão digital e em C++ do jogo de cartas "Tapão". Para desenvolver esse projeto, foi utilizado multithreading (com auxílio da bilbioteca `threads`) e também sockets para conectar clientes ao servidor.