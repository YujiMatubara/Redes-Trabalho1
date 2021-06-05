#ifndef SOCKET_SERVER.H
#define SOCKET_SERVER.H

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <string>

void * sendMsg();
void * listener;
int setServerSocket();
awaitPlayersConnection();

#endif