#include "gameLogicServer.h"

class Game {
    private:
        const char cardsSequence = {'A', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'J', 'Q', 'K'};
        std::vector<card> deck;
        std::vector<card> usedDeck;
        std::vector<card> stack;
        int curDeckSize;
        int activePlayersNB = MAX_PLAYERS;
        std::vector<_player> activePlayers;

    public:
        Game(std::vector<int>);
        void initializePlayers(std::vector<int>);

        void createDeck();
        void shuffleDeck();
        void giveCards();
        int lose();
        void newRound();
        int runGame();

};