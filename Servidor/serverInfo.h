#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>
#include<pthread.h>

// Naipes: ordenadas de mais fortes para menos fortes
#define PAUS 0
#define COPAS 1
#define ESPADAS 2
#define OUROS 3

#define DECK_SIZE 40 // quantidade de cartas no baralho
#define NB_PLAYERS 4 // número de jogadores

#define MSG_SIZE 256

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

#endif