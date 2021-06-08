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

#include "gameLogicServer.hpp"

#define DECK_SIZE 52 // quantidade de cartas no baralho

#define PAUS 0
#define COPAS 1
#define ESPADAS 2
#define OURO 3

#define MSG_SIZE 256

void Game::initializePlayers(std::vector<int> playersSocket){
    int curSocket;
    bool trueVar = true;
    activePlayersNB = playersSocket.size();
    
    cardsPerPlayer =  52/activePlayersNB;
    curDeckSize = cardsPerPlayer*activePlayersNB; 
    usedDeck.resize(curDeckSize);
    
    for(int i = 0;i<curDeckSize;i++) usedDeck.at(i) = deck.at(i);

    for(int i = 0; i < activePlayersNB; i++){
        curSocket = playersSocket.at(i);
        activePlayers[curSocket] = _player{};
        activePlayers[curSocket].myTurn = !trueVar;
        activePlayers[curSocket].active = trueVar;
        activePlayers[curSocket].socket = curSocket;
        activePlayers[curSocket].cardsInHand = cardsPerPlayer;
    }
    activePlayers[playersSocket.at(0)].myTurn = true;
    giveCards();
}

Game::Game(std::unordered_map<int, int> playersSocketPair) {
    std::vector<int> playersSocket;
    for(auto sockets : playersSocketPair){
        playersSocket.push_back(sockets.second);
    }
    createDeck();
    initializePlayers(playersSocket);
}

// Função que adiciona todas as cartas ao baralho (13 de cada naipe)
void Game::createDeck() {
    for (int suit = 0; suit < 4; suit++) {
        for (int j = 0; j < 13; j++) {
            card addToDeck = { suit, cardsSequence[j].first };
            deck.push_back(addToDeck);
        }
    }
    printf("Deck inicializado\n");

    shuffleDeck();
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

    //int val = 3;
    for (auto personSocket : activePlayers){
        playersSequence.push_back(personSocket.first);       
        for(int i=0;i<cardsPerPlayer;i++){
            personSocket.second.deck.push_back(usedDeck.back());
            usedDeck.pop_back();
        }
    }

    printf("Cartas distribuidas\n");
    return;
}

std::unordered_map<int,std::string> Game::cardPlayed(int personId){
    desiredCardId = (desiredCardId+1)%(cardsSequence.size());
    topCard = activePlayers[personId].deck.front();
    activePlayers[personId].deck.pop_front();
    stack.push_front(topCard);


    activePlayers[personId].cardsInHand --;
    activePlayers[personId].myTurn = false;
    curPlayerIndex = (curPlayerIndex+1)%activePlayersNB;
    activePlayers[playersSequence.at(curPlayerIndex)].myTurn = true;
    tapQtty = 0;

    return (activePlayers[personId].cardsInHand == 0 ? sendEndGameMessage(personId) : getAllClientsMessages());
}

bool Game::willGainCards(){
    bool isCorrectCard = (cardsSequence[desiredCardId].first == topCard.value); 
    return isCorrectCard ? (++tapQtty == activePlayersNB) : (tapQtty++ == 0);
}

std::unordered_map<int,std::string> Game::cardTapped(int personId) {
    if(willGainCards()){
        // O jogador que perde fica com a pilha de cartas
        activePlayers[personId].cardsInHand += stack.size();
        while(!stack.empty()){
            activePlayers[personId].deck.push_back(stack.back());
            stack.pop_back();
        }
    }

    return getAllClientsMessages();
}

std::string Game::getCardName(_card c){
    return c.value + "_" + suitIdToStr[c.suit];
}

std::string Game::getClientMessage(int personId){
    auto person = activePlayers[personId];
    std::string message = "nextCardName#"+ getCardName(person.deck.front()) + "|nextActivePlayerID#" + std::to_string(playersSequence.at(curPlayerIndex)) + "|nbCardsInHand#";

    // nbCardsInHand#10,3,4,5,6
    for(int i = 0; i < activePlayersNB; i++) {
        message += std::to_string(activePlayers[playersSequence[i]].cardsInHand);
        message += ( i < activePlayersNB - 1 ? "," : "");
    }
    return message;
}



std::unordered_map<int,std::string> Game::getAllClientsMessages(){
    std::unordered_map<int,std::string> allMessages;
    int socketId;
    for(int i=0;i<activePlayersNB;i++){
        socketId = playersSequence.at(i);
        allMessages[socketId] = getClientMessage(socketId);
    }
    return allMessages;
}

std::unordered_map<int, std::string> Game::sendEndGameMessage(int personId) {
    std::unordered_map<int,std::string> allMessages;
    std::string endGameMessage = "endGame#Fim de jogo, o vencedor é o jogador de id " + std::to_string(personId);
    int socketId;
    for(int i=0;i<activePlayersNB;i++){
        socketId = playersSequence.at(i);
        allMessages[socketId] = endGameMessage;
    }

    return allMessages;
}