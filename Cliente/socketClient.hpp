#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <unordered_map>

int initializeSocket();
int startConnection(int socketClient, int port, const char ip[15]);
void sendMessage(int socketClient, const char * message);
std::string receiveMessage(int socketClient);// fim mutex lock