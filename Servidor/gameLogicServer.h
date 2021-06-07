#ifndef GAME_LOGIC_SERVER_H
#define GAME_LOGIC_SERVER_H

#include "serverInfo.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <chrono> // std::chrono::system_clock
#include <vector>

void createDeck();
void suffleDeck();
void giveCards();
int lose(); // FUNÇÃO TEMPORÁRIA
void newRound();
int runGame();

#endif