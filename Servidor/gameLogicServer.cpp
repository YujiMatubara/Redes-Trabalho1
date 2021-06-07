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
#include "gameLogicServer.hpp"

void Game::initializePlayers(std::vector<int> playersSocket){
    activePlayersNB = playersSocket.size();
    activePlayers.resize(activePlayersNB);
    
    int cardsPerPlayer =  52/activePlayersNB;
    curDeckSize = cardsPerPlayer*activePlayersNB; 
    usedDeck.resize(curDeckSize);
    
    for(int i = 0;i<curDeckSize;i++) usedDeck.at(i) = deck.at(i);

    giveCards();
    for(int i = 0; i < activePlayersNB; i++){
        activePlayers.at(i).socket = playersSocket.at(i);
        activePlayers.at(i).cardsInHand = cardsPerPlayer;
    }
}


Game::Game(std::vector<int> playersSocket) {
    createDeck();
    initializePlayers(playersSocket);
}

// Função que adiciona todas as cartas ao baralho (13 de cada naipe)
void Game::createDeck() {
    for (int suit = 0; suit < 4; suit++) {
        for (int j = 0; j < 13; j++) {
            card addToDeck = { suit, cardsSequence[j] };
            deck.push_back(addToDeck);
        }
    }
    
    printf("Deck inicializado\n");

    return;
}

// Função que dá suffle no baralho
void Game::shuffleDeck() {
    // Time-based seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Embaralhando deck
    shuffle(deck.begin(), deck.end(), std::default_random_engine(seed)); //default_random_engine gera números pseudo-randômicos

    printf("Deck embaralhado\n");
    return;
}

// Função que distribui as cartas aos jogadores
void Game::giveCards() {
    // Distribui as cartas até o deck acabar

    int currPlayer = 0;
    while(!usedDeck.empty()) {
        // Tira uma carta do final do usedDeck e coloca na mão do jogador
        activePlayers[currPlayer].cardsInHand++;
        activePlayers[currPlayer].deck.push_back(usedDeck.back());
        printf("Distribuiu carta %c para o jogador %d\n", usedDeck.back().value, currPlayer);
        usedDeck.pop_back();

        // Prepara para distribuir a carta para o próximo jogador
        currPlayer = (currPlayer+1)%activePlayersNB;
    }

    printf("Cartas distribuidas\n");
    return;
}

// Função temporária para emular uma resposta do socket sobre quem bateu por último
int Game::lose() {
    return rand()%2;
}

// O funcionamento principal do jogo ocorre dentro desta etapa
void Server::newRound() {
    printf("Início do round\n");
    int currPlayer = 0;
    bool endGame = false;

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
            stack.push_back(activePlayers[currPlayer].deck.back());
            activePlayers[currPlayer].deck.pop_back();
            activePlayers[currPlayer].cardsInHand--;
            printf("Jogador %d está com %d cartas restantes\n", currPlayer, activePlayers[currPlayer].cardsInHand);

            /* ================================
            Atualizar a tela mostrando a nova carta no topo da pilha
            ==================================*/
            
            /* ================================
            Obter o jogador que bateu por último ou bateu na carta errada para dar o monte
            ==================================*/

            // Se alguém bateu por último ou bateu errado
            if(lose()) {
                printf("Alguém perdeu\n");
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
                    activePlayers[losingPlayer].deck.push_back(stack.back());
                    stack.pop_back();
                }

                if(activePlayers[currPlayer].cardsInHand == 0)
                    printf("O jogador %d venceu\n", currPlayer);

                currPlayer = losingPlayer;
                
            }
            else {
                printf("Ninguém perdeu\n");
                // Vai para o próximo jogador
                currPlayer = (currPlayer+1)%activePlayersNB;
            }
        }
        else
            // Vai para o próximo jogador
            currPlayer = (currPlayer+1)%activePlayersNB;

    }

    return;
}

// Função que inicia o jogo
int Game::runGame() {
    /* ========================================================
    if (setServerSocket() != 0) {
        std::cout << "Erro. Jogo não pôde ser inicializado\n";
        return;
    }
    ======================================================*/

    // Zera o cardsInHand de todos os jogadores
    for(int i = 0; i < activePlayersNB; i++) 
        activePlayers[i].cardsInHand = 0;

    // Espera conexões dos clientes e verificar se 4 players estão conectados ao servidor
    /*==========================================================
    awaitPlayersConnection();
    ===========================================================*/

    // Monta o deck e inicia a partida
    createDeck();
    shuffleDeck();
    giveCards();
    newRound();

    return 0;
}