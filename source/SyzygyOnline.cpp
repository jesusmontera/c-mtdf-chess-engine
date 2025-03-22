
// Thanks to author Weiss UCI compliant chess engine.
//  Copyright (C) 2023 Terje Kirstihagen

#include "SyzygyOnline.h"
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <winsock2.h>
#undef INFINITE
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define HOSTENT struct hostent
#define IN_ADDR struct in_addr
#define SOCKADDR_IN struct sockaddr_in
#define SOCKET_ERROR -1
#define WSADATA int
#define WSAStartup(a, b) (*b = 0)
#define WSACleanup()
#endif


SyzygyOnline::SyzygyOnline()
{
    //ctor
}

SyzygyOnline::~SyzygyOnline()
{
    //dtor
}
bool SyzygyOnline::query_lichess_syzygy(const chess::Board &board, chess::Move &move)
{
    puts("info string OnlineSyzygy: Querying lichess for a tablebase move...");

    //query lichess syzygy api
    char *hostname = "tablebase.lichess.ovh";
    char *msg_fmt = "GET http://tablebase.lichess.ovh/standard?fen=%s\n";
    char message[256];
    std::string sfen = board.getFen();
    snprintf(message, 256, msg_fmt, sfen.c_str());

    char *response = Query(hostname, message);
    //std::cerr << "# Syzyg RESPONSE\n " << response << std::endl;
            // On success the response includes "uci": "[MOVE]"

    if (strstr(response, "uci") == nullptr)
        return false;

    char *pcategory=strstr(response, "category");
    if (pcategory == nullptr)
        return false;

    std::string s_category(pcategory + 11, 7);
    if (s_category=="unknown"){
            std::cerr << "# Syzyg CATEGORY UNKNOWN" << std::endl;
            return false;
    }


    char *moveInfo = strstr(response, "uci");
    int length=0;
    while(length < 6){
        if (moveInfo[length+6]=='"') break;
        length++;
    }
    std::string uci_move(moveInfo + 6, length);
    std::cerr << "# Syzyg MOVE " << uci_move << " category " << s_category << std::endl;
    move = chess::uci::uciToMove(board, uci_move);

    /**move = ParseMove(moveInfo + 6, pos);
    *dtz = atoi(strstr(moveInfo, "dtz") + 5);
    *wdl = strstr(response, "category")[11] == 'w' ? TB_WIN
         : strstr(response, "category")[11] == 'l' ? TB_LOSS
                                                   : TB_DRAW;
    */
    puts("info string OnlineSyzygy: Move received");

    return true;
}



static void error(const char *msg) { perror(msg); exit(0); }

char * SyzygyOnline::Query(char *hostname, char *message) const
{

    // Setup sockets on windows, does nothing on linux
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
        error("WSAStartup failed.");

    // Replace spaces with +
    char *ptr;
    while ((ptr = strchr(message + 4, ' ')) != NULL)
        *ptr = '+';

    printf("info string Query: %s", message);

    // Create socket
    SOCKET sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        error("ERROR opening socket");

    // Lookup IP address
    HOSTENT *hostent;
    if ((hostent = gethostbyname(hostname)) == NULL)
        error("ERROR no such host");

    // Fill in server struct
    SOCKADDR_IN server = { 0 };
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    server.sin_addr.s_addr = *(uint64_t *)hostent->h_addr;

    // Connect
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        error("ERROR connecting");

    // Send message
    if (send(sockfd, message, strlen(message), 0) < 0)
        error("ERROR sending");

    // Receive response
    static char response[16384];
    memset(response, 0, sizeof(response));
    if (recv(sockfd, response, sizeof(response), 0) == SOCKET_ERROR)
        error("ERROR receiving");

    // Cleanup
    close(sockfd);
    WSACleanup();

    return puts("info string Query: Response received"), response;
}
