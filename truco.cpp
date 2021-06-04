//criar o jogo aqui

/* funcoes
 * criar baralho
 * criar jogadores
 * embaralhar
 * dar cartas
 * ordenar valor por manilha
 * jogar rounds 1, 2, 3
 * computar pontos
 * end loop
 * if pontos == 12 show:vencedor
 * 
 * Algumas regras:
 * Sequência da maior para a menor: 3 2 A K J Q 7 6 5 4 (de todos os naipes)
 * Mão: A mão (como se fosse uma rodada) vale 1 ponto. Pode ser aumentada para 3, 6, 9 e 12
 * Jogo termina em 12 pontos
 * É possível aumentar a aposta da rodada de 3 em três. Uma dupla pede 3, a outra pode pedir 6, a primeira pede 9 e a segunda pede 12
 * Mão de Onze: Se a dupla tiver 11 pontos, eles podem olhar as mãos uns dos outros
 * Mão de Ferro: Caso ambas as duplas tenham 11 pontos, todos recebem as cartas viradas e devem jogar assim
 * É permitido esconder a carta que você joga na mesa, exceto na primeira mão de cada rodada]
 * No final da distribuição das cartas, vira-se uma carta para cima e a carta que de acordo com a sequência é a próxima, em seus 4 diferentes naipes, são definidas como as Manilhas
 * A ordem de “força” obedece o naipe, da seguinte maneira (do maior para o menor): Paus > Copas > Espadas > Ouros
 * 
 * Número de cartas: 40 
 * 3 cartas para cada participante
 * 4 Jogadores
 */

#include <iostream>
#include <random>
#include <algorithm>
#include <chrono> // std::chrono::system_clock
#include <vector>

// Para o socket
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define DECK_SIZE 40
#define NB_PLAYERS 4

const char cards[] = { '3' , '2', 'A', 'K', 'J', 'Q', '7', '6', '5', '4'}; // ordenadas de mais fortes para menos fortes
const int endRoundPonts = 12;

// Naipes: ordenadas de mais fortes para menos fortes
#define PAUS 0
#define COPAS 1
#define ESPADAS 2
#define OUROS 3

typedef struct _card {
    int suit;
    char value;
} card;

typedef struct _player {
    std::string name;
    int teamID; // só há 2 times
    card cardsInHand[3];
    int socket;
} player;

class Server {
    private:
        std::vector<card> deck;
        std::vector<card> currRoundDeck; // deck usado na rodada (algumas cartas serão removidas conforme elas são dadas aos jogadores)
        char trumpValue; // manilha
        int roundPoints;
        player activePlayers[NB_PLAYERS];

        int socketServer;
        struct sockaddr_in addr;
        pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
        int done = 1;

    public:
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

    // ================ Funções relacionadas ao jogo ================
    // Função que adiciona todas às 40 cartas ao baralho (10 de cada naipe)
    void createDeck() {
        for (int suit = 0; suit < 4; suit++) {
            for (int j = 0; j < 10; j++) {
                card addToDeck = { suit, cards[j] };
                deck.push_back(addToDeck);
            }
        }
        
        std::cout << "Deck inicializado\n";

        return;
    }

    // Função que dá suffle no baralho
    void suffleDeck() {
        // Time-based seed
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        // Embaralhando deck
        shuffle(deck.begin(), deck.end(), std::default_random_engine(seed)); //default_random_engine gera números pseudo-randômicos

        return;
    }

    void newRound() {
        // Seta a pontuação inicial da rodada como 0
        roundPoints = 0;

        // Embaralhar deck
        suffleDeck();

        // Selecionar carta que "foi virada" e será removida da rodada
        int removedCardSuit = rand() % 4; // 4 naipes possíveis
        int removedCardIndex = rand() % 10; // 10 valores de carta possíveis
        card removedCard = { removedCardSuit, cards[removedCardIndex]};

        // Selecionar manilhas da rodada
        trumpValue = cards[removedCardIndex+1 % 10]; // próxima carta (após a retirada) representa a manilha
        std::cout << "Manilha definida: " << trumpValue << std::endl;

        currRoundDeck = deck; // com vectores, esse "=" cria uma cópia do vetor
        currRoundDeck.erase(currRoundDeck.begin() + removedCardIndex); // removendo carta do baralho da rodada
        

        return;
    }

    // Função que dá 3 cartas a um jogador
    void giveCards(card * playerCards) {
        // Dando 3 cartas ao jogador
        for (int cardNb = 0; cardNb < 3; cardNb++) {
            int selectedCardSuit = rand() % 4; // 4 naipes possíveis
            int valueIndex = rand() % 10;
            char selectedCardValue = cards[valueIndex]; // 10 valores de carta possíveis

            card selectedCard = { selectedCardSuit, selectedCardValue };

            // Checa se a carta selecionada existe no vetor, ou seja, ainda pode ser dada para um jogador
            if (std::find(currRoundDeck.begin(), currRoundDeck.end(), selectedCard) != currRoundDeck.end() ) {
                playerCards[cardNb] = selectedCard;
                currRoundDeck.erase(currRoundDeck.begin() + valueIndex); // removendo carta que será dada ao jogador do baralho da rodada
            }
            printf("Carta %c do náipe %d entregue ao jogador\n", selectedCard.value, selectedCard.suit);
        }
    } 

    // Função que inicia o jogo
    void startGame() {
        if (setServerSocket() != 0) {
            std::cout << "Erro. Jogo não pôde ser inicializado\n";
            return;
        }

        // Passo 1: espera conexões dos clientes e verificar se 4 players estão conectados ao servidor
        awaitPlayersConnection();

        // Passo 2: formar 2 duplas e informar aos jogadores isso
        for (int pIndex = 0; pIndex < NB_PLAYERS; pIndex++) {
            if (pIndex < (int) NB_PLAYERS/2)
                activePlayers[pIndex].teamID = 0;
            else
                activePlayers[pIndex].teamID = 1;
            printf("Jogador <%d> inserido no time %d\n", activePlayers[pIndex].name, activePlayers[pIndex].teamID);
        }
        // falta informar os jogadores sobre as duplas

        // Passo 3: começar 1ª rodada
        createDeck();
        newRound(); // escolhe manilha da rodada e embaralha deck

        // Passo 4: distribuir cartas aos jogadores e informar sobre a manilha
        for (int playerIndex = 0; playerIndex < NB_PLAYERS; playerIndex++) {
            std::cout << "Dando cartas ao jogador " << playerIndex << std::endl;
            giveCards(activePlayers[playerIndex].cardsInHand);

            // é preciso avisar ao jogadores quais as cartas que ele recebeu
            // enviar pelo Socket

            // falta avisar aos jogadores sobre a manilha
        }


        // Passo 5: chamar função que aguarda input dos jogadores e gerencia o jogo...
    }

};

