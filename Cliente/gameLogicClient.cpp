#include "gameLogicClient.hpp"
#include "socketClient.hpp"

std::map< std::string, std::string> cardDrawings;
std::mutex mtx;
bool gameRunning; // flag que verifica se o jogo começou
int playerID = -1, socketID, nbPlayers, nextActivePlayerID; // nextActivePlayerID =  jogador que fará o comando de jogar a carta
std::string nextCardName, desiredCard; // carta que será jogada nessa rodada
std::vector<int> nbCardsInHand, lastNbCardsInHand; // qtd de cartas na mão de cada jogador
bool threadRun = false; // flag que fala se as threads devem ou não rodar
int nextCardIndex, deckSize = 0, clientSocket;
void loadCardDrawings() {
    // Cartas de paus
    cardDrawings.insert(std::make_pair("A_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    A    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    2    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    3    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    4    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    5    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    6    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    7    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    8    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    9    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("0_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    10   ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    J    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♣║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_paus", "╔═════════╗\n║♣        ║\n║         ║\n║    K    ║\n║         ║\n║        ♣║\n╚═════════╝"));

    // Cartas de copas
    cardDrawings.insert(std::make_pair("A_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    A    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    2    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    3    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    4    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    5    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    6    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    7    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    8    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    9    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("0_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    10   ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    J    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♥║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_copas", "╔═════════╗\n║♥        ║\n║         ║\n║    K    ║\n║         ║\n║        ♥║\n╚═════════╝"));

    // Cartas de espadas
    cardDrawings.insert(std::make_pair("A_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    A    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    2    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    3    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    4    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    5    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    6    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    7    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    8    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    9    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("0_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    10   ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    J    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♠║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_espadas", "╔═════════╗\n║♠        ║\n║         ║\n║    K    ║\n║         ║\n║        ♠║\n╚═════════╝"));

    // Cartas de ouros
    cardDrawings.insert(std::make_pair("A_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    A    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("2_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    2    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("3_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    3    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("4_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    4    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("5_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    5    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("6_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    6    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("7_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    7    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("8_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    8    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("9_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    9    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("0_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    10   ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("J_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    J    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("Q_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    Q    ║\n║         ║\n║        ♦║\n╚═════════╝"));
    cardDrawings.insert(std::make_pair("K_ouros", "╔═════════╗\n║♦        ║\n║         ║\n║    K    ║\n║         ║\n║        ♦║\n╚═════════╝"));

return;
}

// Função que calcula o total de cartas na mão de todos os jogadores (soma total)
int getTotalCards() {
    int total = 0;
    for (int cardsInHand : nbCardsInHand)
        total += cardsInHand;
    return total;
}

void printCardVariationMsg(int currPlayer) {
    if (nbCardsInHand[currPlayer] > lastNbCardsInHand[currPlayer]) {
        std::cout << RED << "++++++ " << nbCardsInHand[currPlayer] - lastNbCardsInHand[currPlayer] << " ++++++" << RESET << std::endl;
    }
    else if (nbCardsInHand[currPlayer] < lastNbCardsInHand[currPlayer]) {
        std::cout << GREEN << "------ " << lastNbCardsInHand[currPlayer] - nbCardsInHand[currPlayer] << " ------" << RESET << std::endl;
    }

    return;
}

// Função que mostra todos os elementos do jogo na tela do cliente (nº de cartas do jogador, carta atual na mesa etc.)
void showScreenElements() {
    // "Limpa" a tela
    printf(" ================================ TAPÃO ================================ \n\n");
    printf("[ PlayerID = %d - socketID = %d ]\n\n", playerID, socketID);  //jogador da vez

    printCardVariationMsg(playerID); // printa mensagem caso tenha ocorrido variação na qtd. de cartas na mão do jogador
    
    printf("\nNúmero de jogadores: %d\n", nbPlayers);
    for (int i = 0; i < nbPlayers; i++) {
        if (i == playerID)
            std::cout << YELLOW << "[VOCÊ]";    // Destaca qual jogador é você
        
        printf("\t%3dº jogador: %3d cartas na mão", i+1, nbCardsInHand[i]); //cartas
        
        if (nextActivePlayerID == i)
            std::cout << RED << " <= jogador da vez" << RESET;  //se for outro player
        
        printf("\n");
        
        if (i == playerID)
            std::cout << RESET;
    }

    std::cout << BLUE;
    if (desiredCard == "0")
        desiredCard = "10";
    printf("\nCarta esperada: %s\n", desiredCard.c_str());
    std::cout << RESET;
    
    std::string currCard = cardDrawings[nextCardName];
    std::cout << currCard << std::endl;
    std::cout << "Total de cartas no monte: " << deckSize - getTotalCards() << std::endl;
    printf("\n");
    printf("Digite x para jogar carta ou Enter para dar o Tapão:\n");
}

// Função que espera input do usuário (só 3 comandos/chars são aceitos)
// ENTER = bater na mesa
// x = jogar carta
char getKeyPress() {
    char pressedKey;
    while (1) {
        pressedKey = std::cin.get();
        if (pressedKey == 10) {
            break;
        }
        else if (pressedKey == 'x' || pressedKey == 'q' || pressedKey == 's') {
            std::cin.get(); // pega o ENTER depois dos caracters (ou EOF se o primeiro char já for um ENTER)
            break;    
        } 
        else {
            std::cin.get(); // pega o ENTER depois dos caracters (ou EOF se o primeiro char já for um ENTER)
        }   
    }

    return pressedKey;
}

// Função que cria um mapa não ordenado com os valores recebidos do servidor
std::unordered_map<std::string, std::string> createMap(std::string serverResponse) {
    std::unordered_map<std::string, std::string> messageServer;
    serverResponse[serverResponse.size()-1] = '|'; // mensagem deve terminar com um delimitador

    //std::string serverResponse ("playerNo#5|lastCardPlayer#A_paus|sla1#sla2|");
    std::string firstDelimiter ("|"); // delimitadores para a função substr
    std::string secondDelimiter ("#");
    int pos; //posição do delimitador
    int line; //posição inicial da linha, por exemplo: |assss|assssssss neste caso eh a pos dos 'a'

    std::vector<std::string> lineVector; //criando um vector para dividir pelo primeiro delimitador
    pos = serverResponse.find(firstDelimiter); //procura pela primeira ocorrência do '|'
    lineVector.push_back(serverResponse.substr(0, pos)); //insere no vetor a primeira linha
    line = pos+1; //passa para a próxima linha
    while(true) {
        pos = serverResponse.find(firstDelimiter, line);    //procura pelo primeiro delimitador a linha
        if (pos == -1)   //para caso encontre o fim da linha
            break;
        lineVector.push_back(serverResponse.substr(line, pos - line));  //adiciona no vector a linha
        line = pos+1;   //passa para a proxima linha
    }

    for (size_t i = 0; i < lineVector.size(); i++)
    {
        pos = lineVector.at(i).find(secondDelimiter);   //pega a posição do delimitador # no vetor
        messageServer[lineVector.at(i).substr(0, pos)] = lineVector.at(i).substr(pos + 1, lineVector.at(i).size()); //insere no map
    }

    return messageServer;
}

// Função que fica esperando a mensagem via socket do servidor indicando início do jogo
void * waitStartGameSignal() {
    // o servidor vai enviar o estado do início de jogo
    printf("Espere os jogadores se conectarem e aperte Enter.\n");
    std::string serverResponse = receiveMessage(clientSocket);

    //quebrando a resposta do servidor em variáveis separadas
    std::unordered_map < std::string, std::string > serverRespFiltered = createMap(serverResponse);
    
    mtx.lock(); // mutex para atualizar alguns valores
    if (serverRespFiltered.find("playerSocket") != serverRespFiltered.end() && serverRespFiltered.find("nbPlayers") != serverRespFiltered.end() && serverRespFiltered.find("playerID") != serverRespFiltered.end()) {
        nbPlayers = atoi(serverRespFiltered["nbPlayers"].c_str());  //nbplayers está como string
        playerID = atoi(serverRespFiltered["playerID"].c_str());    //playerid está como string
        socketID = atoi(serverRespFiltered["playerSocket"].c_str());
        // updateGameState(serverRespFiltered);
        gameRunning = true;
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
        nbCardsInHand.push_back(atoi(stringNbCardsInHand.substr(start, end - start).c_str()));  //transforma a carta em número e coloca no vector
        start = end + delimiter.size(); //pegar o inicio da linha
        end = stringNbCardsInHand.find(delimiter, start);   //pegar o final da linha
    }
    nbCardsInHand.push_back(atoi(stringNbCardsInHand.substr(start, stringNbCardsInHand.size() - start).c_str())); // último elemento, sem delimitador após ele

    return nbCardsInHand;
}

// Função que atualiza as variáveis relacionadas ao estado atual do jogo
// Ao atualizar o estado do jogo, atualiza a interface
// Obs.: SEMPRE USAR MUTEX AO CHAMAR ESSA FUNÇÃO!
bool updateGameState(std::unordered_map < std::string, std::string > & serverRespFiltered) {
    bool ok = true;

    if (serverRespFiltered.find("nbPlayers") != serverRespFiltered.end()) { //checa se a mensagem foi corretamente recebida
        nbPlayers = atoi(serverRespFiltered["nbPlayers"].c_str());  //recebe valor em inteiro
    }
    else
        ok = false; // valor não pôde ser atualizado, algo pode estar errado
    
    if (serverRespFiltered.find("nbCardsInHand") != serverRespFiltered.end()) { // Atualiza o número de cartas na mão de quem jogou por último
        std::string stringNbCardsInHand = serverRespFiltered["nbCardsInHand"];
        lastNbCardsInHand = nbCardsInHand;
        nbCardsInHand = splitStringIntoInts(stringNbCardsInHand, ",");
    }
    else
        ok = false;

    if (serverRespFiltered.find("nextCardName") != serverRespFiltered.end()) { // Atualiza qual é a próxima carta do topo
        nextCardName = serverRespFiltered["nextCardName"];
    }
    else
        ok = false;

    if (serverRespFiltered.find("nextActivePlayerID") != serverRespFiltered.end()) { // Atualiza qual o próximo jogador a jogar
        nextActivePlayerID = atoi(serverRespFiltered["nextActivePlayerID"].c_str());
    }
    else
        ok = false;
    
    if (serverRespFiltered.find("desiredCardID") != serverRespFiltered.end()) { // Atualiza qual a carta esperada
        desiredCard = serverRespFiltered["desiredCardID"];
    }
    else
        ok = false;
    
    if (serverRespFiltered.find("playerID") != serverRespFiltered.end()) { // Atualiza o ID do jogador atual
        playerID = atoi(serverRespFiltered["playerID"].c_str());
    }
    else
        ok = false;
    
    if (serverRespFiltered.find("playerSocket") != serverRespFiltered.end()) { // Atualiza o socket ID atual
        socketID = atoi(serverRespFiltered["playerSocket"].c_str());
    }
    else
        ok = false;

    return ok;
}

// Função do socket que escuta o servidor
void * listenServer() {
    while (threadRun) { //enquanto a thread estiver ativa
        if (!threadRun) break;

        std::string serverResponse = receiveMessage(clientSocket);  //captura a mensagem do servidor pela função do socketClient
        std::unordered_map < std::string, std::string > serverRespFiltered = createMap(serverResponse);   //cria um map que splita os valores do jogo recebidos

        if (serverRespFiltered.find("endGame") != serverRespFiltered.end()) { // se no map enviado pelo servidor há um endGame, o jogo finalizou
            gameRunning = false;
            printf("\n\n\n\n\n\n%s\n", serverRespFiltered["endGame"].c_str()); // mensagem de fim de jogo
            break;
        }
        
        // atualiza "nbPlayers|nextCardName|nextActivePlayerID|nbCardsInHand_Player0|nbCardsInHand_Player1|...|nbCardsInHand_PlayerN"
        mtx.lock(); // mutex aqui para atualizar os valores
        updateGameState(serverRespFiltered);
        if (deckSize == 0) { // calcular a qtd de cartas no baralho a partir de quantas cartas os jogadores tem na mão
            deckSize = (int)(52/nbPlayers)*nbPlayers;
        }
        mtx.unlock();
        
        showScreenElements(); // atualiza a tela para o usuário
        // sleep(0.3); 
    }

    printf("Matando thread de listenServer()\n");
    std::terminate();

    return 0;
}

// Função que envia mensagens para o servidor (as ações que o jogador pode fazer)
void * sendMsg() {
    while (threadRun) {
        if (!threadRun) break;

        char keyPressed = getKeyPress();
        // chama função que envia mensagem para o socket -> enviar keyPressed

        if (!gameRunning) { // se o jogo não está jogando, pressionar enter começa o jogo
            if (keyPressed == 10) // só é permitido mandar ENTER para começar o jogo
                sendMessage(clientSocket, "start_game");
            continue; // outras entradas não são permitidas
        }
        
        mtx.lock();
        if (keyPressed == 10) {// ENTER -> comando para bater na mesa
            sendMessage(clientSocket, "slam_table");
            std::string serverResponse = receiveMessage(clientSocket);
            std::unordered_map < std::string, std::string > serverRespFiltered = createMap(serverResponse);
            updateGameState(serverRespFiltered); // atualiza os valores após "bater na mesa"
        }
        else if (keyPressed == 'x') { // pressionou o 'x' -> comando de baixar carta
            if (nextActivePlayerID == playerID) { // se o próximo player for o playerID, ele pode enviar o comando de baixar carta
                sendMessage(clientSocket, "play_card");
                std::string serverResponse = receiveMessage(clientSocket);
                std::unordered_map < std::string, std::string > serverRespFiltered = createMap(serverResponse);
                updateGameState(serverRespFiltered); // atualiza os valores após "bater na mesa"
            }
        }
        else if (keyPressed == 'q') { // pressionou o 'q'
            sendMessage(clientSocket, "quit");
            gameRunning = false; // game ended
        }
        mtx.unlock();
    }

    printf("Matando thread de sendMsg()\n");
    std::terminate();

    return 0;
}




void terminateAll(int signum) {
    close(clientSocket);
    printf("[!] Terminando cliente\n");
    exit(signum);  
}

void handleSignals(){
    std::vector<int> handledSignals = {SIGTERM,SIGSEGV,SIGINT,SIGILL,SIGABRT,SIGFPE};
    for(auto _signal : handledSignals) signal(_signal,terminateAll);
}



//Funcao que executa o jogo no cliente
int main(int argc, char const *argv[]) {
    handleSignals();
    gameRunning = false;

    if (argc < 3) {
        printf("[ERRO] É necessário passar IP e PORTA na qual você deseja conectar-se!\n[?] Como usar: ./cliente [IP] [PORTA]\n");
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
    clientSocket = initializeSocket();  //iniciar socket do cliente
    
    if (startConnection(clientSocket, serverPort, serverIP.c_str()) != 0) { //conectar ao servidor
        std::cout << "ERRO ao conectar com o servidor!\n";
        return 1;
    }
    printf("[*] Conectado em %s:%d!\n", serverIP.c_str(), serverPort);

    printf("\n[*] Pressione ENTER caso queira começar o jogo ou\n");
    printf("[*] Espere um sinal de início de jogo\n");

    threadRun = true; // flag que fala para as threads ficarem rodando

    // Thread 2 => fica esperando entrada do usuário ( getKeyPress() ) e, dependendo do nextActivePlayerID, permite que o usuário envio o comando de baixar carta
    // O comando de bater na mesa está disponível sempre
    // Se o jogo não tiver começado, o cliente pode enviar um comando de start_game" para o servidor
    std::thread sendMsgThread(sendMsg);
    
    // Thread 3 => "morre" rápido, só serve para ficar escutando uma mensagem do servidor falando que o jogo iniciou
    std::thread waitStartSignal(waitStartGameSignal);

    while (!gameRunning) { if(gameRunning) break; sleep(0.3);}  // espera sinal de começo de jogo ou alguem jogador pressionar ENTER para começar o game

    // Tread 1 => espera mensagem do servidor e vai atualizando as informações do estado do jogo
    // Se o nextActivePlayerID for o playerID, é permitido que esse cliente envie mensagem para o servidor
    std::thread listenServerThread(listenServer);

    for (int i = 0; i < nbPlayers; i++) { // inicializa cada jogador com 0 cartas (essa informação virá do servidor no futuro)
        nbCardsInHand.push_back(0);
        lastNbCardsInHand.push_back(0); // mantém um tracking da qtd. de cartas na rodada anterior
    }

     if (playerID == -1) {
        std::cout << "ERRO ao tentar definir o PlayerID.\n";
        return 1;
    }

    printf("[*] O jogo irá começar!!\n");

    loadCardDrawings(); // salva o desenho das cartas em um array

    // showScreenElements(); // atualiza a tela para o usuário

    while (gameRunning) { if(!gameRunning) break; sleep(0.3); }
    
    mtx.lock();
    threadRun = false; // avisa para as threads que elas devem ser finalizadas
    mtx.unlock();

    listenServerThread.join();
    sendMsgThread.join();
    
    close(clientSocket);
    printf("[!] Jogo finalizado! Obrigado por participar :)\n");

    return 0;
}