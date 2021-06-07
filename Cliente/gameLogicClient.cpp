#include "gameLogicClient.hpp"
#include "socketClient.hpp"

std::map< std::string, std::string> cardDrawings;
std::mutex mtx;
bool gameRunning; // flag que verifica se o jogo começou
int playerID = -1, nbPlayers, nextActivePlayerID; // nextActivePlayerID =  jogador que fará o comando de jogar a carta
std::string nextCardName; // carta que será jogada nessa rodada
std::vector<int> nbCardsInHand; // qtd de cartas na mão de cada jogador

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
        if (pressedKey == 10 || pressedKey == 'x' || pressedKey == 'q') {
            break;
        }
        std::cin.get(); // pega o ENTER depois dos caracters (ou EOF se o primeiro char já for um ENTER)
    }

    return pressedKey;
}

/* Ordem do que deve ser feito:
1) Conectar ao servidor -> arquivo "socketClient"
2) Ficar esperando sinal de início de jogo
3) Chamar funcão que espera mensagem do servidor e escuta entradas do usuário
*/

// Função que cria um mapa nao ordenado com os valores recebidos do servidor
std::unordered_map<std::string, std::string> createMap(int clientSocket) {
    std::unordered_map<std::string, std::string> messageServer;
    std::string serverResponse = receiveMessage(clientSocket);
    //std::string serverResponse ("playerNo#5|lastCardPlayer#A_paus|sla1#sla2|");
    std::string firstDelimiter ("|");   //delimitadores para a funcao substr
    std::string secondDelimiter ("#");
    int pos;    //posicao do delimitador
    int line;   //posicao inicial da linha, por exemplo: |assss|assssssss neste caso eh a pos dos 'a'

    std::vector<std::string> lineVector;    //criando um vector para dividir pelo primeiro delimitador
    pos = serverResponse.find(firstDelimiter);  //procura pela primeira ocorrencia do '|'
    lineVector.push_back(serverResponse.substr(0, pos));    //insere no vetor a primeira linha
    line = pos+1;   //passa para a proxima linha
    while(true) {
        pos = serverResponse.find(firstDelimiter, line);    //procura pelo primeiro delimitador a linha
        if (pos == std::string::npos)   //para caso encontre o fim da linha
        {   
            break;
        }
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
bool waitStartGameSignal(int clientSocket) {
    // o servidor vai enviar o estado do início de jogo
    bool ok = true;

    std::unordered_map<std::string, std::string> funcaoYuji = createMap(clientSocket);

    //quebrando a resposta do servidor em variaveis separadas
    std::unordered_map < std::string, std::string > serverRespFiltered = createMap(clientSocket, serverRespFiltered);
    mtx.lock(); // mutex para atualizar alguns valores
    if (serverRespFiltered.find("nbPlayers") != serverRespFiltered.end() && serverRespFiltered.find("playerID") != serverRespFiltered.end()) {
        nbPlayers = atoi(serverRespFiltered["nbPlayers"].c_str());
        playerID = atoi(serverRespFiltered["playerID"].c_str());

        ok = true;
    }
    else {
        ok = false;
    }
    mtx.unlock();

    return ok;

}

std::vector<int> splitNbCardsInHand(std::string stringNbCardsInHand) {
    std::vector<int> nbCardsInHand;

    return nbCardsInHand;
}

// Função que atualiza as variáveis relacionadas ao estado atual do jogo
// Obs.: SEMPRE USAR MUTEX AO CHAMAR ESSA FUNÇÃO!
int updateGameState(std::unordered_map < std::string, std::string > & serverRespFiltered) {
    bool ok = true;

    if (serverRespFiltered.find("nbPlayers") != serverRespFiltered.end())
        nbPlayers = atoi(serverRespFiltered["nbPlayers"].c_str());
    else
        ok = false; // valor não pôde ser atualizado, algo está errado
    
    if (serverRespFiltered.find("nbCardsInHand") != serverRespFiltered.end()) {
        std::string stringNbCardsInHand = serverRespFiltered["nbCardsInHand"];
        nbCardsInHand = splitNbCardsInHand(stringNbCardsInHand);
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

    return ok;
}

void * listenServer(int clientSocket) {
    // chama função do socket que escuta servidor
    std::string serverResponse = receiveMessage(clientSocket);
    std::unordered_map < std::string, std::string > serverRespFiltered = funcaoDoYujiAqui();
    
    // atualiza "nbPlayers|nextCardName|nextActivePlayerID|nbCardsInHand_Player0|nbCardsInHand_Player1|...|nbCardsInHand_PlayerN"
    mtx.lock(); // mutex aqui para atualizar os valores
    if (!updateGameState(serverRespFiltered)) {
        std::cout << "ERRO ao atualizar as variáveis de estado do servidor\n";
    }
    mtx.unlock();

    return 0;
}

void * sendMsg(int clientSocket) {
    char keyPressed = getKeyPress();
    // chama função que envia mensagem para o socket -> enviar keyPressed
    
    mtx.lock();
    if (keyPressed == 10) {// ENTER -> comando para bater na mesa
        sendMessage(clientSocket, "slam_table");
        std::string serverResponse = receiveMessage(clientSocket);
        std::unordered_map < std::string, std::string > serverRespFiltered = funcaoDoYujiAqui();
        updateGameState(serverRespFiltered); // atualiza os valores após "bater na mesa"
    }
    else if (keyPressed = 'x') { // pressionou o 'x' -> comando de baixar carta
        if (nextActivePlayerID == playerID) { // se o próximo player for o playerID, ele pode enviar o comando de baixar carta
            sendMessage(clientSocket, "play_card");
            std::string serverResponse = receiveMessage(clientSocket);
        std::unordered_map < std::string, std::string > serverRespFiltered = funcaoDoYujiAqui();
        updateGameState(serverRespFiltered); // atualiza os valores após "bater na mesa"
        }
    }
    else { // pressionou o 'q'
        sendMessage(clientSocket, "quit");
        gameRunning = false; // game ended
    }
    mtx.unlock();

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

    printf("[*] Esperando sinal de início de jogo\n");

    while (!gameRunning) {
        if (waitStartGameSignal(clientSocket) == true) // espera sinal de começo de jogo
            gameRunning = true;
    }

     if (playerID == -1) {
        std::cout << "ERRO ao tentar definir o PlayerID.\n";
        return 1;
    }

    printf("[*] O jogo irá começar!!\n");

    loadCardDrawings(); // salva o desenho das cartas em um array

    // Tread 1 => espera mensagem do servidor e vai atualizando as informações do estado do jogo
    // Se o nextActivePlayerID for o playerID, é permitido que esse cliente envie mensagem para o servidor
    std::thread listenServerThread( listenServer, clientSocket, &nbPlayers, nextCardName, &nextActivePlayerID, nbCardsInHand );
    
    // Thread 2 => fica esperando entrada do usuário ( getKeyPress() ) e, dependendo do nextActivePlayerID, permite que o usuário envio o comando de baixar carta
    // O comando de bater na mesa está disponível sempre
    std::thread sendMsgThread( sendMsg, clientSocket, playerID, &nbPlayers, nextCardName, &nextActivePlayerID, nbCardsInHand );

    while (gameRunning) {}
    // int nbCardsInHand[4] = { 10, 7, 8, 15};
    // showScreenElements(1, nbCardsInHand, "A_Ouros", 4, 3);
    // getKeyPress();
    return 0;
}
