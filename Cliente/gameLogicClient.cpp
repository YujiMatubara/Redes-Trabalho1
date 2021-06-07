#include "gameLogicClient.hpp"

std::map< std::string, std::string> cardDrawings;

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
void showScreenElements(int playerID, int * cardsInHand, std::string currCardName, int nbPlayers, int activePlayerID) {
    // "Limpa" a tela
    for (int n = 0; n < 4; n++)
        printf("\n\n\n\n\n\n\n\n\n\n");
    printf(" ================================ TAPÃO ================================ \n\n");
    printf("[ JOGADOR %d ]\n\n", playerID);
    
    printf("\nNúmero de jogadores: %d\n", nbPlayers);
    for (int i = 0; i < nbPlayers; i++) {
        if (i == playerID)
            std::cout << YELLOW << "[VOCÊ]";
        
        printf("\tJogador %3d: %3d cartas na mão", i, cardsInHand[i]);
        
        if (activePlayerID == i)
            std::cout << RED << " <= jogador da vez" << RESET;
        
        printf("\n");
        
        if (i == playerID)
            std::cout << RESET;
    }
    printf("\n\n");
    
    std::string currCard = cardDrawings[currCardName];
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

// Função que fica esperando mensagem via socket do servidor indicando início do jogo
bool waitStartGameSignal(int * nbPlayers, std::string & nextCardName, vector<int> & cardsInHand) {
    // provavelmente isso é uma função que fica no arquivo de socket
    // o servidor vai enviar o estado do início de jogo
    // Informações relevantes: nª de jogadores, cartas na mão de cada jogador, próxima carta a ser jogada
    // Recebe do servidor "nbPlayers|nextCardName|cardsInHand_Player0|cardsInHand_Player1|...|cardsInHand_PlayerN"

}

void * listen() {
    // chama função do socket aqui que escuta servidor
    // atualiza "nbPlayers|nextCardName|cardsInHand_Player0|cardsInHand_Player1|...|cardsInHand_PlayerN"
}

void * sendMsg() {
    char keyPressed = getKeyPress();
    // chama função que envia mensagem para o socket -> enviar keyPressed
}

int main(int argc, char const *argv[]) {
    int playerID, nbPlayers;
    std::string nextCardName;
    vector<int> cardsInHand;

    if (argc < 3) {
        printf("[ERRO] É necessário passar IP e PORTA no qual você deseja conectar-se!\n[?] Como usar: ./cliente [IP] [PORTA]\n");
        return 0;
    }

    std::string serverIP = argv[1];
    int serverPort = atoi(argv[2]);
    
    printf("[*] Tentando conectar-se à %s:%d...\n", serverIP.c_str(), serverPort);
    // Conecta via socket com o servidor em IP:port
    // ===========================
    // Conexão via socket aqui
    // --> PlayerID é definido aqui <--
    // ===========================


    printf("[*] Esperando sinal de início de jogo");
    while (waitStartGameSignal(&nbPlayers, nextCardName, cardsInHand)) {}; // espera sinal de começo de jogo

    loadCardDrawings(); // salva o desenho das cartas em um array

    // [[ thread aqui ]]
    // Tread 1 => espera mensagem do servidor e vai atualizando o nextActivePlayer
    // Se o nextActivePlayer for o playerID, é permitido que esse cliente envie mensagem para o servidor
    // Thread 2 => fica esperando entrada do usuário ( getKeyPress() ) e, dependendo do nextActivePlayer, permite que o usuário envio o comando de baixar carta
    // O comando de bater na mesa é disponível sempre
    std::thread listenServerThread( listen );
    std::thread sendMsgThread( sendMsg );

    int cardsInHand[4] = { 10, 7, 8, 15};
    showScreenElements(1, cardsInHand, "A_Ouros", 4, 3);
    getKeyPress();
    return 0;
}
