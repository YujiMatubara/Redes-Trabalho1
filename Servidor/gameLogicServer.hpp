#pragma once
#include <unordered_map>
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono> // std::chrono::system_clock
#include <vector>
#include <list>
// Funções removidas do serverInfo.h
#include <string>
#include <pthread.h>

typedef struct _card {
    int suit;
    char value;
} card;

typedef struct _player {
    std::list<_card> deck;
    int cardsInHand;
    int socket;
    bool active;
    bool myTurn;
} player;

class Game {
    private:
        std::vector<int> playersSequence;
        int curPlayerIndex = 0;
        
        _card topCard;
        int desiredCardId = 0;
        int tapQtty;
        std::unordered_map<int,std::string> suitIdToStr = {{0,"paus"},{1,"copas"},{2,"espadas"},{3,"ouros"}};
        std::vector<std::pair<char,std::string>> cardsSequence = {{'A',"Az"}, {'2',"Dois"}, {'3',"Três"}, {'4',"Quatro"}, {'5',"Cinco"}, {'6',"Seis"}, {'7', "Sete"}, {'8', "Oito"}, {'9', "Nove"}, {'0', "Dez"}, {'J', "Valete"}, {'Q', "Rainha"}, {'K', "Rei"}};
        std::vector<_card> deck;
        std::vector<_card> usedDeck;
        std::list<_card> stack;
        int cardsPerPlayer;
        int curDeckSize;
        int activePlayersNB;
        std::unordered_map<int,_player> activePlayers;

    public:
        std::string getCardName(_card);
        Game(std::unordered_map<int,int>);
        void initializePlayers(std::vector<int>);

        void createDeck();
        void shuffleDeck();
        void giveCards();
        void newRound();
        std::unordered_map<int,std::string> cardPlayed(int);
        std::unordered_map<int,std::string> cardTapped(int); 
        bool willGainCards();
        std::string getClientMessage(int);
        std::unordered_map<int,std::string> getAllClientsMessages();
        std::unordered_map<int,std::string> sendEndGameMessage(int);
        std::string getCardName(_card);

};