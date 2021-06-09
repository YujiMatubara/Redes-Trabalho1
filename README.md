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

- Cliente: make client
- Servidor: make server

Para executar o cliente, basta dar o comando `./client <IP_DO_SERVIDOR> <PORTA_DO_SERVIDOR>` dentro da pasta `Cliente`, sendo o servidor o endereço na qual você deseja conectar-se.

Já para executar o servidor, basta dar o comando `./server` dentro da pasta `Servidor`.
    
# Breve descrição do trabalho:
Nesse trabalho, foi desenvolvido uma versão digital e em C++ do jogo de cartas "Tapão". 
O jogo se baseia em jogadores, cada um em sua vez, jogars cartas no topo de uma pilha. Ao mesmo tempo que colocam é falado o nome de uma carta, o próximo jogador deve falar a carta seguinte. Quando a carta falada é a mesma que a última jogada, os jogadores devem bater na pilha. O último a bater leva as cartas.Caso batam em momento errado, a primeira pessoa que bater leva o monte. Quem terminar o jogo sem cartas ganha. Para começar o jogo basta o cliente dar `enter` até o jogo começar. Na fase de jogo o jogador na sua vez deve digitar `x` e dar enter. Para bater a qualquer momento basta apertar `enter`.
Para desenvolver esse projeto, foi utilizado multithreading (com auxílio da bilbioteca `threads`) e também sockets para conectar clientes ao servidor.