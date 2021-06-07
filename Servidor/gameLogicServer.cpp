/* 
    Tapão
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
    
    printf("Deck inicializado\n");

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
    while(!deck.empty()) {
        
        // Tira uma carta do final do deck e coloca na mão do jogador
        int index = activePlayers[currPlayers].cardsInHand++;
        activePlayers[currPlayers].hand[index] = deck.back();
        deck.pop_back();

        // Prepara para distribuir a carta para o próximo jogador
        currPlayer = (currPlayer+1)%activePlayersNB;
    }

    return;
}

// Função temporária para emular uma resposta do socket
int lose() {
    return rand()%1;
}

// O funcionamento principal do jogo ocorre dentro desta etapa
void newRound() {
    int currPlayer = 0;
    bool endGame = false
    std::vector<card> stack;

    // A partida se repete enquanto ninguém ficar com todas as cartas
    while(!endGame) {
        
        if(activePlayers[currPlayer].cardsInHand != 0) {
            // Se o jogador ainda está na partida

            if(activePlayers[currPlayer].cardsInHand == DECK_SIZE) {
                // Se alguém ficou com todas as cartas (perdeu)
                printf("O jogador %d perdeu a partida\n", currPlayer);
                endGame = true;
                return;
            }

            /* ================================
            Pegar a carta escolhida pelo jogador por meio do socket
            ==================================*/

            // Tira a carta do deck do jogador e coloca na pilha
            stack.push_back(activePlayers[currPlayer].hand.back());
            activePlayers[currPlayer].hand.pop_back();
            activePlayers[currPlayer].cardsInHand--;

            /* ================================
            Atualizar a tela mostrando a nova carta no topo da pilha
            ==================================*/
            
            /* ================================
            Obter o jogador que bateu por último ou bateu na carta errada para dar o monte
            ==================================*/

            // Se alguém bateu por último ou bateu errado
            if(lose()) {
                int losingPlayer; // Armazena o índice do jogador que perdeu
                /*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                ESSE DO WHILE É PROVISÓRIO, para substituir a escolha do jogador com socket*/
                do {
                    losingPlayer = rand()%activePlayersNB;
                } while (activePlayers[losingPlayer].cardsInHand == 0);
                //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

                // O jogador que perde fica com a pilha de cartas
                activePlayers[losingPlayer].cardsInHand += stack.size();
                while(!stack.empty()) {
                    activePlayers[losingPlayer].hand.push_back(stack.back());
                    stack.pop_back();
                }

                if(activePlayers[currPlayer].cardsInHand == 0)
                    printf("O jogador %d venceu\n", currPlayer);

                currPlayer = losingPlayer;
                
            }
            else
                // Vai para o próximo jogador
                currPlayer = (currPlayer+1)%activePlayersNB;
        }
        else
            // Vai para o próximo jogador
            currPlayer = (currPlayer+1)%activePlayersNB;

    }

    return;
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