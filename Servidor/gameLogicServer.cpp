/* 
    Presidente
 * Algumas regras:
 *
 *  Todos os jogadores recebem um monte de cartas, e elas devem ser jogadas de costas, para que os valores sejam
 * descobertos apenas no momento da jogada. Cada jogador fala uma carta da ordem de ás até réi, e coloca a carta
 * do topo do monte na mesa. O jogador seguinte fala a próxima carta da sequência, ou ás caso a carta falada antes
 * seja um rei, e joga uma carta do topo do baralho. Se a carta falada for a mesma que a tirada do topo, todos os
 * jogadores precisam bater a mão no monte, e aquele que bater por último recebe a pilha de cartas jogadas. Ganha
 * os jogadores que ficarem sem nenhuma carta antes do último. 
 *
 * Número de cartas: 52
 * As cartas são divididas por igual entre os jogadores
 * 2 a 6 Jogadores
 */

#include "gameLogicServer.h"
#include "socketServer.h"

const char cardsSequence[] = { 'A' , '2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K'};

std::vector<card> deck;
std::vector<card> currRoundDeck; // cópia do deck usada para a distribuição
player activePlayers[MAX_PLAYERS];
int activePlayersNB = MAX_PLAYERS;

// ================ Funções relacionadas ao jogo ================
// Função que adiciona todas as cartas ao baralho (13 de cada naipe)
void createDeck() {
    for (int suit = 0; suit < 4; suit++) {
        for (int j = 0; j < 13; j++) {
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

// Função que distribui as cartas aos jogadores
void giveCards() {
    // Distribui as cartas até o deck acabar

    int currPlayer = 0;
    while(!currRoundDeck.empty()) {
        
        // Tira uma carta do final do deck e coloca na mão do jogador
        card selectedCard = currRoundDeck.back();
        currRoundDeck.pop_back();

        activePlayers[currPlayers].hand[activePlayers[currPlayers].cardsInHand++] = selectedCard;
        currPlayer = (currPlayer+1)%activePlayersNB;
        printf("Carta %c do náipe %d entregue ao jogador %d\n", selectedCard.value, selectedCard.suit, currPlayer);
    }

}

// O funcionamento principal do jogo ocorre dentro desta etapa
void newRound() {
    int currPlayer = 0;
    std::vector<card> stack;

    // A partida se repete enquanto tiver pelo menos dois jogadores
    while(currPlayersNB > 1) {
        
        Boolean endedRound = false;
        // Laço que se repete até que alguém fique com o monte
        while(!endedRound) {

            if(activePlayers[currPlayer].cardsInHand != 0) {
                // Se o jogador ainda não ganhou

                /* ================================
                Pegar a carta escolhida pelo jogador por meio do socket
                ==================================*/

                // Tira a carta do monte do jogador e coloca na pilha
                card selectedCard = activePlayers[currPlayers].hand.back();
                activePlayers[currPlayers].hand.pop_back();
                activePlayers[currPlayers].cardsInHand--;


                /* ================================
                Atualizar a tela mostrando a selectedCard no topo
                ==================================*/
                stack.push_back(selectedCard);
                
                /* ================================
                Obter o jogador que bateu por último ou bateu na carta errada
                ==================================*/
                int losingPlayer;
                // ESSE DO WHILE É PROVISÓRIO, para substituir a escolha do jogador
                do {
                    losingPlayer = rand() % activePlayersNB;
                } while (activePlayers[losingPlayer].cardsInHand == 0);
            }
            currPlayer = (currPlayer+1)%activePlayersNB;
        }

    }
}

// Função que inicia o jogo
void startGame() {
    if (setServerSocket() != 0) {
        std::cout << "Erro. Jogo não pôde ser inicializado\n";
        return;
    }

    // Zera o cardsInHand de todos os jogadores
    for(int i = 0; i < activePlayersNB; i++) 
        activePlayers[i].cardsInHand = 0;

    // Espera conexões dos clientes e verificar se 4 players estão conectados ao servidor
    awaitPlayersConnection();

    // Monta o deck e inicia a partida
    createDeck();
    newRound();

}