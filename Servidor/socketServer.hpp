#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <string>
#include <string.h>
#include <thread>
//#include <pthread.h>
#include <vector>
#include <mutex>
#include <unordered_map>
#include "gameLogicServer.hpp"
class Server{
    private:
        std::string gamePhase;
        int socketServer;
        struct sockaddr_in addr;
        std::unordered_map<int,int> activePlayers;
        int serverPort;
        int curClientsNo;
        std::unordered_map<int,std::thread> threads;
        std::vector<long> toDelThreads;
        std::mutex mtx;
        int maxPlayers;
        long threadId;
        bool startGame;
        bool gameRuning = false;
        Game *game;

    public:
        Server(int,int);
        Server();
        void changeGamePhase(std::string);
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
        std::string msgStr(int);
        bool continueThread(char*,int*,int);
        void preGameStart(int,std::string);
        void onGame(int,std::string);
        void playerLogMessage(int,int);
        void gameStartMessage();

    private:
        void (Server::*desiredFunct)(int,std::string);
        
        std::unordered_map<std::string,void(Server::*)(int,std::string)> serverPhases{
            {"preGameStart",&Server::preGameStart},
            {"onGame",&Server::onGame}
        };
        //slam_table play_card 
        std::unordered_map<std::string,std::unordered_map<int,std::string>(Game::*)(int)> gameActions{
            {"play_card",&Game::cardPlayed},
            {"slam_table",&Game::cardTapped}
        };    
};