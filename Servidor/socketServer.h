#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "serverInfo.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <string>
#include<pthread.h>

void * connectionHandler(void * socketDesc);
void sendMsg(player * currPlayer, char * msg);
void listener(player * currPlayer);
int setServerSocket();
int awaitPlayersConnection(player * activePlayers);
void closeServer(player * activePlayers);

#endif