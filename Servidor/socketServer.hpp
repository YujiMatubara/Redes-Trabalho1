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
#include <csignal>

void terminateAll(int);

class Server{
    private:
        bool endGame;
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
        int gamePlayers;
        Game *game;

    public:
        ~Server();
        Server(int,int);
        Server();
        void changeGamePhase(std::string);
        void* connectionHandler(int);
        void createThread(int);
        void acceptPlayer(int);
        void setAddr();
        bool validateValue(int,std::string);
        int setServerSocket();
        void awaitPlayersConnection();
        void afterPlayersConnection();
        int gameCycle();
        void deleteOldThreads();
        void closeServer();
        void initializeServer(int,int);
        void endingThread(int,int,int);
        void treatMessages(char*,int,int);
        std::string msgStr(int);
        bool continueThread(char*,int*,int);
        void preGameStart(int,std::string);
        void onGame(int,std::string);
        void playerLogMessage(int,int);
        void gameStartMessage();
        void clearGame();
        void handleSignals();

    private:
        void (Server::*serverPhasesFunct)(int,std::string);
        
        void (Server::*gameCyclesFunct)();


        std::unordered_map<std::string,void(Server::*)(int,std::string)> serverPhases{
            {"preGameStart",&Server::preGameStart},
            {"onGame",&Server::onGame}
        };
        std::unordered_map<std::string,void(Server::*)()> gameCycles{
            {"preGameStart",&Server::awaitPlayersConnection},
            {"onGame",&Server::afterPlayersConnection}
        };
        //slam_table play_card 
        std::unordered_map<std::string,std::unordered_map<int,std::string>(Game::*)(int)> gameActions{
            {"play_card",&Game::cardPlayed},
            {"slam_table",&Game::cardTapped}
        };    
};