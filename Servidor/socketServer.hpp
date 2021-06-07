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
        int socketServer;
        struct sockaddr_in addr;
        int nbClients;
        std::unordered_map<long,player> activePlayers;
        int playersNo;
        int serverPort;
        int curClientsNo;
        std::unordered_map<long,std::thread> threads;
        struct sockaddr_storage serverStorage;
        std::vector<long> toDelThreads;
        socklen_t addrSize;
        std::mutex mtx;
        int maxPlayers;
        long threadId;

    public:
        Server(int,int);
        Server();
        void* connectionHandler(int,int);
        void sendMsg(player*,char*);
        void setAddr();
        bool validateValue(int,std::string);
        void listener(player*);
        int setServerSocket();
        int awaitPlayersConnection();
        void deleteOldThreads();
        void closeServer(player*);
        void initializeServer(int,int);

};