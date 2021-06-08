#ifndef SOCKET_CLIENT_HPP
#define SOCKET_CLIENT_HPP

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
#include <cstring>

int initializeSocket();
int startConnection(int, int, const char *);
void sendMessage(int, const char *);
std::string receiveMessage(int);

#endif