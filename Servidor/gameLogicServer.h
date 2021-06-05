#ifndef GAME_LOGIC_SERVER.H
#define GAME_LOGIC_SERVER.H

#include <iostream>
#include <random>
#include <algorithm>
#include <chrono> // std::chrono::system_clock
#include <vector>

typedef struct _card {
    int suit;
    char value;
} card;

typedef struct _player {
    std::string name;
    int teamID; // só há 2 times
    _card cardsInHand[3];
    int socket;
} player;

void createDeck();
void suffleDeck();
void newRound();
void giveCards(card * playerCards);
void startGame();

#endif