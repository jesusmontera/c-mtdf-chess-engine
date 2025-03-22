#include <iostream>

using namespace std;
#include "NumbFishProbe.h"
#include <thread>

void cmd_position(std::istringstream &is, Board &board)
{
    std::string token;
    is >> token;
    std::string fen;
    //std::string x = is.str();
    //info_to_file(x.c_str());

    if (token == "startpos") {
        fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        board.setFen(fen);
        while (is >> token){
            if (token != "moves"){
                Move mov = uci::uciToMove(board, token);
                board.makeMove(mov);
            }

        }
    } else {

        while (is >> token && token != "moves") {
            fen += token + " ";
        }
        board.setFen(fen);
        while (is >> token) {
            Move mov = uci::uciToMove(board, token);
            board.makeMove(mov);
            //uci_to_move(token)
        }

    }


}


//void cmd_go(std::istringstream &is, const std::string &fen, NumbFishProbe *nbfish )
void cmd_go(std::istringstream &is, const Board &board, NumbFishProbe *nbfish )
{
    std::string token;
    double moveTime = 1000.0; //milisecs from UCI
    while (is >> token) {
        if (token == "movetime") is >> moveTime;
    }

    // Crear un hilo y pasarle el objeto invocable y el argumento
    std::async(std::launch::async, &NumbFishProbe::get_move, nbfish, board, moveTime/1000., false);

}

void uci_loop(NumbFishProbe &nbfish)
{
    Board board;

     std::string cmd, token,fen;
    do
    {
        if (nbfish.m_state==STATE_END_SEARCH)
        {
            //std::string uci_move = nbfish.get_move(fen, moveTime/1000.);

            std::cout << "info depth " << nbfish.get_depth() << \
                    " score cp "<< nbfish.get_score() << \
                    " nodes " << nbfish.get_nodes() << std::endl;
            std::cout << "bestmove " << nbfish.get_result_move() << std::endl;
            nbfish.m_state=STATE_FREE;

        }

        std::getline(std::cin, cmd);

        std::istringstream is(cmd);
        is >> token;

        if      (token == "uci")     std::cout << "id name Numbfish_disservin\n"
                                               << "id author JAKER\n"
                                               << "uciok"               << std::endl;
        else if (token == "isready") std::cout << "readyok"             << std::endl;
        else if (token == "position"){
                cmd_position(is,board);

        }
        else if (token == "go"){
            if (nbfish.m_state==STATE_FREE){
                cmd_go(is,board,&nbfish);
                //cmd_go(is,board.getFen(),&nbfish);
            }
        }else if (token == "stop"){
            nbfish.m_state=STATE_STOP;
        }




    } while (cmd != "quit");

}

int main()
{

    NumbFishProbe nbfish(true,false); // nnue and book
    uci_loop(nbfish);
    return 0;
}






/*
int main()
{

//char fen[]="r3r1k1/pp3pp1/1p3n1p/nq2p3/1P6/PQ1PPBPP/1B3P2/R4RK1 w - - 3 18";
//char fen[]="3q2rk/3b1pbp/p2Pp2Q/2p2p2/1r6/PBN3P1/1PP2P1P/K2R3R w - - 1 21";

//char fen[]="6k1/2R2pp1/p3p3/1p2Bb1n/1P5P/PB6/5K2/7r w - - 0 31";

//char fen[]="r1bqkb1r/p4ppp/2p2n2/nB2p1N1/8/8/PPPP1PPP/RNBQK2R w KQkq - 0 1";

//char fen[]="r1bqk2r/pp1nbNp1/2p1p2p/8/2BP4/1PN3P1/P3QP1P/3R1RK1 b kq - 0 19";
//  char fen[]="7r/2k5/2b5/R7/8/8/2r3P1/3R2K1 w - - 0 1";
  char fen[]="2r3k1/4npr1/2q5/pp5Q/3p3p/PB1P3P/1PP2PPB/4R1K1 w - - 0 37";
    //char fen[]= "8/8/8/4K3/2R5/1R6/6k1/8 w - - 0 1";
    //char fen[]="8/8/8/4k3/2r5/1r6/6K1/8 b - - 0 1";



   Board board(fen);
   //CPosition pos;
   //pos.set_board(board);
   //std::cout << "\n" << pos.is_valid_board() <<"\n";
   //pos.print_pos();

   NumbFishProbe nbfish(true,false); // nnue and book

    nbfish.get_move(board,5.0,true);
    cout << "\n RESULT MOVE " << nbfish.get_result_move();
    //cout << "\nclaves_score_reusadas " << nbfish.m_claves_score_reusadas;
    return 0;
}
*/

