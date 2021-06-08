#include "gameLogicClient.hpp"
#include "socketClient.hpp"

// g++ -Wall gameLogicClient.cpp socketClient.cpp -o testGameLogicClient -lpthread 

std::map< std::string, std::string> cardDrawings;
std::mutex mtx;
bool gameRunning; // flag que verifica se o jogo começou
int playerID = -1, nbPlayers, nextActivePlayerID; // nextActivePlayerID =  jogador que fará o comando de jogar a carta
std::string nextCardName; // carta que será jogada nessa rodada
std::vector<int> nbCardsInHand; // qtd de cartas na mão de cada jogador
bool threadRun = false;

void loadCardDrawings() {
    // Cartas de paus
    cardDrawings.insert(std::make_pair("A_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    A    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    2    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    3    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    4    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    5    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    6    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    7    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    8    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    9    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("10_Paus","╔═════════╗\n║♣        ║\n║         ║\n║    10   ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    J    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_Paus", "╔═════════╗\n║♣        ║\n║         ║\n║    K    ║\n║         ║\n║        ♣║\n╚═════════╝"));

    // Cartas de copas
    cardDrawings.insert(std::make_pair("A_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    A    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    2    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    3    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    4    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    5    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    6    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    7    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    8    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    9    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("10_Copas","╔═════════╗\n║♥        ║\n║         ║\n║    10   ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    J    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_Copas", "╔═════════╗\n║♥        ║\n║         ║\n║    K    ║\n║         ║\n║        ♥║\n╚═════════╝"));

    // Cartas de espadas
    cardDrawings.insert(std::make_pair("A_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    A    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    2    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    3    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    4    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    5    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    6    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    7    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    8    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    9    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("10_Espadas","╔═════════╗\n║♠        ║\n║         ║\n║    10   ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    J    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_Espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    K    ║\n║         ║\n║        ♠║\n╚═════════╝"));

    // Cartas de ouros
    cardDrawings.insert(std::make_pair("A_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    A    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    2    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    3    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    4    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    5    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    6    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    7    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    8    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    9    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("10_Ouros","╔═════════╗\n║♦        ║\n║         ║\n║    10   ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    J    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_Ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    K    ║\n║         ║\n║        ♦║\n╚═════════╝"));

return;
}

// Função que mostra todos os elementos do jogo na tela do cliente (nº de cartas do jogador, carta atual na mesa etc.)
void showScreenElements() {
    // "Limpa" a tela
    for (int n = 0; n < 4; n++)
        printf("\n\n\n\n\n\n\n\n\n\n");
    printf(" ================================ TAPÃO ================================ \n\n");
    printf("[ JOGADOR %d ]\n\n", playerID);
    
    printf("\nNúmero de jogadores: %d\n", nbPlayers);
    for (int i = 0; i < nbPlayers; i++) {
        if (i == playerID)
            std::cout << YELLOW << "[VOCÊ]";
        
        printf("\tJogador %3d: %3d cartas na mão", i, nbCardsInHand[i]);
        
        if (nextActivePlayerID == i)
            std::cout << RED << " <= jogador da vez" << RESET;
        
        printf("\n");
        
        if (i == playerID)
            std::cout << RESET;
    }
    printf("\n\n");
    
    std::string currCard = cardDrawings[nextCardName];
    std::cout << currCard << std::endl;
    printf("\n\n");

}

// Função que espera input do usuário (só 2 comandos/chars são aceitos)
// ENTER = bater na mesa
// x = jogar carta
char getKeyPress() {
    char pressedKey;
    while (1) {
        pressedKey = std::cin.get();
        if (pressedKey == 10) {
            break;
        }
        else if (pressedKey == 'x' || pressedKey == 'q') {
            std::cin.get(); // pega o ENTER depois dos caracters (ou EOF se o primeiro char já for um ENTER)
            break;    
        } 
        else {
            std::cin.get(); // pega o ENTER depois dos caracters (ou EOF se o primeiro char já for um ENTER)
        }
    }

    return pressedKey;
}

// Função que cria um mapa nao ordenado com os valores recebidos do servidor
std::unordered_map<std::string, std::string> createMap(int clientSocket, std::string serverResponse) {
    std::unordered_map<std::string, std::string> messageServer;
    serverResponse[serverResponse.size()-1] = '|'; // mensagem deve terminar com um delimitador

    //std::string serverResponse ("playerNo#5|lastCardPlayer#A_paus|sla1#sla2|");
    std::string firstDelimiter ("|"); // delimitadores para a funcao substr
    std::string secondDelimiter ("#");
    int pos; //posicao do delimitador
    int line; //posicao inicial da linha, por exemplo: |assss|assssssss neste caso eh a pos dos 'a'

    std::vector<std::string> lineVector; //criando um vector para dividir pelo primeiro delimitador
    pos = serverResponse.find(firstDelimiter); //procura pela primeira ocorrencia do '|'
    lineVector.push_back(serverResponse.substr(0, pos)); //insere no vetor a primeira linha
    line = pos+1; //passa para a proxima linha
    while(true) {
        pos = serverResponse.find(firstDelimiter, line);    //procura pelo primeiro delimitador a linha
        if (pos == std::string::npos)   //para caso encontre o fim da linha
            break;
        lineVector.push_back(serverResponse.substr(line, pos - line));  //adiciona no vector a linha
        line = pos+1;   //passa para a proxima linha
    }

    for (size_t i = 0; i < lineVector.size(); i++)
    {
        pos = lineVector.at(i).find(secondDelimiter);   //pega a posicao do delimitador # no vetor
        messageServer[lineVector.at(i).substr(0, pos)] = lineVector.at(i).substr(pos + 1, lineVector.at(i).size()); //insere no map
    }

    return messageServer;
}

// Função que fica esperando mensagem via socket do servidor indicando início do jogo
void * waitStartGameSignal(int clientSocket) {
    // o servidor vai enviar o estado do início de jogo
    std::string serverResponse = receiveMessage(clientSocket);
    // std::string serverResponse ("nbPlayers#5|nextCardName#A_paus|playerID#1|nextActivePlayerID#2|nextCardName#A_paus|nextActivePlayerID#2|nbCardsInHand#10,3,4,5,6|");

    //quebrando a resposta do servidor em variaveis separadas
    std::unordered_map < std::string, std::string > serverRespFiltered = createMap(clientSocket, serverResponse);
    for (auto & x : serverRespFiltered) {
        std::cout << "map[" << x.first << "] = " << x.second << std::endl;
    }
    
    mtx.lock(); // mutex para atualizar alguns valores
    if (serverRespFiltered.find("nbPlayers") != serverRespFiltered.end() && serverRespFiltered.find("playerID") != serverRespFiltered.end()) {
        nbPlayers = atoi(serverRespFiltered["nbPlayers"].c_str());
        playerID = atoi(serverRespFiltered["playerID"].c_str());
        gameRunning = true;
        printf("Game irá começar. gameRunning = true\n");
    }
    else {
        printf("ERRO ao setar nº de jogadores e playerID ao iniciar o jogo\n");
    }
    mtx.unlock();

    return 0;

}

// Função que splita uma string em vários ints a partir de um delimitador
std::vector<int> splitStringIntoInts(std::string & stringNbCardsInHand, std::string delimiter) {
    std::vector<int> nbCardsInHand;

    int start = 0;
    int end = stringNbCardsInHand.find(delimiter);
    while (end != -1) {
        // std::cout << stringNbCardsInHand.substr(start, end - start) << std::endl;
        nbCardsInHand.push_back(atoi(stringNbCardsInHand.substr(start, end - start).c_str()));
        start = end + delimiter.size();
        end = stringNbCardsInHand.find(delimiter, start);
    }
    nbCardsInHand.push_back(atoi(stringNbCardsInHand.substr(start, stringNbCardsInHand.size() - start).c_str())); // último elemento, sem delimitador após ele

    return nbCardsInHand;
}

// Função que atualiza as variáveis relacionadas ao estado atual do jogo
// Ao atualizar o estado do jogo, atualiza a interface
// Obs.: SEMPRE USAR MUTEX AO CHAMAR ESSA FUNÇÃO!
int updateGameState(std::unordered_map < std::string, std::string > & serverRespFiltered) {
    bool ok = true;

    if (serverRespFiltered.find("nbPlayers") != serverRespFiltered.end())
        nbPlayers = atoi(serverRespFiltered["nbPlayers"].c_str());
    else
        ok = false; // valor não pôde ser atualizado, algo está errado
    
    if (serverRespFiltered.find("nbCardsInHand") != serverRespFiltered.end()) {
        std::string stringNbCardsInHand = serverRespFiltered["nbCardsInHand"];
        nbCardsInHand = splitStringIntoInts(stringNbCardsInHand, ",");
    }
    else
        ok = false;

    if (serverRespFiltered.find("nextCardName") != serverRespFiltered.end())
        nextCardName = serverRespFiltered["nextCardName"];
    else
        ok = false;

    if (serverRespFiltered.find("nextActivePlayerID") != serverRespFiltered.end())
        nextActivePlayerID = atoi(serverRespFiltered["nextActivePlayerID"].c_str());
    else
        ok = false;
    
    showScreenElements(); // atualiza a tela para o usuário

    return ok;
}

void * listenServer(int clientSocket) {
    while (threadRun) {
        // chama função do socket que escuta servidor
        std::string serverResponse = receiveMessage(clientSocket);
        std::unordered_map < std::string, std::string > serverRespFiltered = createMap(clientSocket, serverResponse);

        if (serverRespFiltered.find("endGame") != serverRespFiltered.end()) { // se no map enviado pelo servidor há um endGame, o jogo finalizou
            gameRunning = false;
            printf("\n\n\n\n\n\n\n\n\n%s\n", serverRespFiltered["endGame"].c_str()); // mensagem de fim de jogo
            return 0;
        }
        
        // atualiza "nbPlayers|nextCardName|nextActivePlayerID|nbCardsInHand_Player0|nbCardsInHand_Player1|...|nbCardsInHand_PlayerN"
        mtx.lock(); // mutex aqui para atualizar os valores
        if (!updateGameState(serverRespFiltered)) {
            std::cout << "ERRO ao atualizar as variáveis de estado do servidor\n";
        }
        mtx.unlock();
        sleep(0.3);
    }

    return 0;
}

void * sendMsg(int clientSocket) {
    while (threadRun) {
        char keyPressed = getKeyPress();
        // chama função que envia mensagem para o socket -> enviar keyPressed

        // std::cout << "Tecla pressionada: <" << keyPressed << ">" << std::endl;
        if (!gameRunning) { // se o jogo não está jogando, pressionar enter começa o jogo
            if (keyPressed == 10) // só é permitido mandar ENTER para começar o jogo
                sendMessage(clientSocket, "start_game");
            continue; // outras entradas não são permitidas
        }
        
        mtx.lock();
        if (keyPressed == 10) {// ENTER -> comando para bater na mesa
            sendMessage(clientSocket, "slam_table");
            std::string serverResponse = receiveMessage(clientSocket);
            std::unordered_map < std::string, std::string > serverRespFiltered = createMap(clientSocket, serverResponse);
            updateGameState(serverRespFiltered); // atualiza os valores após "bater na mesa"
        }
        else if (keyPressed == 'x') { // pressionou o 'x' -> comando de baixar carta
            if (nextActivePlayerID == playerID) { // se o próximo player for o playerID, ele pode enviar o comando de baixar carta
                sendMessage(clientSocket, "play_card");
                std::string serverResponse = receiveMessage(clientSocket);
                std::unordered_map < std::string, std::string > serverRespFiltered = createMap(clientSocket, serverResponse);
                updateGameState(serverRespFiltered); // atualiza os valores após "bater na mesa"
            }
        }
        else { // pressionou o 'q'
            sendMessage(clientSocket, "quit");
            gameRunning = false; // game ended
        }
        mtx.unlock();
    }

    return 0;
}

int main(int argc, char const *argv[]) {
    gameRunning = false;

    if (argc < 3) {
        printf("[ERRO] É necessário passar IP e PORTA no qual você deseja conectar-se!\n[?] Como usar: ./cliente [IP] [PORTA]\n");
        return 0;
    }

    std::string serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    if (serverIP.size() > 16) {
        printf("[!] IP muito longo -> formato aceito: XXX.XXX.XXX.XXX\n");
        return 1;
    }
    
    printf("[*] Tentando conectar-se à %s:%d...\n", serverIP.c_str(), serverPort);
    // Conecta via socket com o servidor em IP:port
    int clientSocket = initializeSocket();  //iniciar socket do cliente
    
    if (startConnection(clientSocket, serverPort, serverIP.c_str()) != 0) { //conectar ao servidor
        std::cout << "ERRO ao conectar com o servidor!\n";
        return 1;
    }
    printf("[*] Conectado em %s:%d!\n", serverIP.c_str(), serverPort);

    printf("\n[*] Pressione ENTER caso queira começar o jogo ou\n");
    printf("[*] Espere um sinal de início de jogo\n");

    threadRun = true; // fala para as threads ficarem rodando

    // Tread 1 => espera mensagem do servidor e vai atualizando as informações do estado do jogo
    // Se o nextActivePlayerID for o playerID, é permitido que esse cliente envie mensagem para o servidor
    std::thread listenServerThread( listenServer, clientSocket );
    
    // Thread 2 => fica esperando entrada do usuário ( getKeyPress() ) e, dependendo do nextActivePlayerID, permite que o usuário envio o comando de baixar carta
    // O comando de bater na mesa está disponível sempre
    // Se o jogo não tiver começado, o cliente pode enviar um comando de "game_start" para o servidor
    std::thread sendMsgThread( sendMsg, clientSocket );

    // Thread 3 => "morre" rápido, só serve para ficar escutando uma mensagem do servidor falando que o jogo iniciou
    std::thread waitStartSignal(waitStartGameSignal, clientSocket);

    while (!gameRunning) { if(gameRunning) break; sleep(0.3);}  // espera sinal de começo de jogo ou alguem jogador pressionar ENTER para começar o game

    for (int i = 0; i < nbPlayers; i++) // inicializa cada jogador com 0 cartas (essa informação virá do servidor no futuro)
        nbCardsInHand.push_back(0);

     if (playerID == -1) {
        std::cout << "ERRO ao tentar definir o PlayerID.\n";
        return 1;
    }

    printf("[*] O jogo irá começar!!\n");

    loadCardDrawings(); // salva o desenho das cartas em um array

    while (gameRunning) {}
    
    mtx.lock();
    threadRun = false; // avisa para as threads que elas devem ser finalizadas
    mtx.unlock();
    
    printf("[!] Jogo finalizado! Obrigado por participar :)\n");

    return 0;
}