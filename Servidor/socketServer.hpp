#pragma once

#include "serverInfo.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <string>
#include <thread>
//#include <pthread.h>
#include <vector>
#include <mutex>
#include <unordered_map>

class Server{
    private:
        bool gameRunning;
        int socketServer;
        struct sockaddr_in addr;
        std::unordered_map<long,player> activePlayers;
        int serverPort;
        int curClientsNo;
        std::unordered_map<long,std::thread> threads;
        std::vector<long> toDelThreads;
        std::mutex mtx;
        int maxPlayers;
        long threadId;
        bool startGame;

    public:
        Server(int,int);
        Server();
        void* connectionHandler(int);
        void createThread(int);
        void acceptPlayer(int);
        void setAddr();
        bool validateValue(int,std::string);
        int setServerSocket();
        int awaitPlayersConnection();
        void deleteOldThreads();
        void closeServer(player*);
        void initializeServer(int,int);
        void endingThread(int,int,int);
        void treatMessages(char*,int,int);
        int msgSize(int);
};