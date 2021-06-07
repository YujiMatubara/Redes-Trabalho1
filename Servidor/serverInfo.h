#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include <string>
#include <pthread.h>
#include <vector>

#define DECK_SIZE 52 // quantidade de cartas no baralho
#define MAX_PLAYERS 6 // número máximo de jogadores

#define PAUS 0
#define COPAS 1
#define ESPADAS 2
#define OURO 3

#define MSG_SIZE 256

typedef struct _card {
    int suit;
    char value;
} card;

typedef struct _player {
    std::vector <struct _card> deck;
    int cardsInHand;
    int socket;
} player;

#endif