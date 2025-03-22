// Position.cpp: implementation of the CPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "CPosition.h"
#include <iostream> // para cout


const int g_pst[2][6][64]={{{100,100,100,100,100,100,100,100,69,108,93,63,64,86,103,69,78,109,105,89,90,98,103,81,74,103,110,109,106,101,100,77,83,116,98,115,114,100,115,87,107,129,121,144,140,131,144,107,178,183,186,173,202,182,185,190,100,100,100,100,100,100,100,100},
{206,257,254,256,261,245,258,211,257,265,282,280,282,280,257,260,262,290,293,302,298,295,291,266,279,285,311,301,302,315,282,280,304,304,325,317,313,321,305,297,290,347,281,354,353,307,342,278,277,274,380,244,284,342,276,266,214,227,205,205,270,225,222,210},
{313,322,305,308,306,305,310,310,339,340,331,326,327,326,340,336,334,345,344,335,328,345,340,335,333,330,337,343,337,336,320,327,345,337,340,354,346,345,335,330,311,359,288,361,372,310,348,306,309,340,355,278,281,351,322,298,261,242,238,244,297,213,283,270},
{449,455,461,484,477,461,448,447,426,441,448,453,450,436,435,426,437,451,437,454,454,444,453,433,451,444,463,458,466,450,433,449,479,484,495,492,497,475,470,473,498,514,507,512,524,506,504,494,534,508,535,546,534,541,513,539,514,508,512,483,516,512,535,529},
{890,899,898,916,898,893,895,887,893,911,929,910,914,914,908,891,899,923,916,918,913,918,913,902,915,914,927,924,928,919,909,907,930,913,951,946,954,949,916,923,927,972,961,989,1001,992,972,931,943,961,989,919,949,1005,986,953,935,930,921,825,998,953,1017,955},
{60017,60030,59997,59986,60006,59999,60040,60018,59996,60003,59986,59950,59943,59982,60013,60004,59953,59958,59957,59921,59936,59968,59971,59968,59945,59957,59948,59972,59949,59953,59992,59950,59945,60050,60011,59996,59981,60013,60000,59951,59938,60012,59943,60044,59933,60028,60037,59969,59968,60010,60055,60056,60056,60055,60010,60003,60004,60054,60047,59901,59901,60060,60083,59938},
},
{{100,100,100,100,100,100,100,100,190,185,182,202,173,186,183,178,107,144,131,140,144,121,129,107,87,115,100,114,115,98,116,83,77,100,101,106,109,110,103,74,81,103,98,90,89,105,109,78,69,103,86,64,63,93,108,69,100,100,100,100,100,100,100,100},
{210,222,225,270,205,205,227,214,266,276,342,284,244,380,274,277,278,342,307,353,354,281,347,290,297,305,321,313,317,325,304,304,280,282,315,302,301,311,285,279,266,291,295,298,302,293,290,262,260,257,280,282,280,282,265,257,211,258,245,261,256,254,257,206},
{270,283,213,297,244,238,242,261,298,322,351,281,278,355,340,309,306,348,310,372,361,288,359,311,330,335,345,346,354,340,337,345,327,320,336,337,343,337,330,333,335,340,345,328,335,344,345,334,336,340,326,327,326,331,340,339,310,310,305,306,308,305,322,313},
{529,535,512,516,483,512,508,514,539,513,541,534,546,535,508,534,494,504,506,524,512,507,514,498,473,470,475,497,492,495,484,479,449,433,450,466,458,463,444,451,433,453,444,454,454,437,451,437,426,435,436,450,453,448,441,426,447,448,461,477,484,461,455,449},
{955,1017,953,998,825,921,930,935,953,986,1005,949,919,989,961,943,931,972,992,1001,989,961,972,927,923,916,949,954,946,951,913,930,907,909,919,928,924,927,914,915,902,913,918,913,918,916,923,899,891,908,914,914,910,929,911,893,887,895,893,898,916,898,899,890},
{59938,60083,60060,59901,59901,60047,60054,60004,60003,60010,60055,60056,60056,60055,60010,59968,59969,60037,60028,59933,60044,59943,60012,59938,59951,60000,60013,59981,59996,60011,60050,59945,59950,59992,59953,59949,59972,59948,59957,59945,59968,59971,59968,59936,59921,59957,59958,59953,60004,60013,59982,59943,59950,59986,60003,59996,60018,60040,59999,60006,59986,59997,60030,60017},
}};

/*
int parse(char c[2]) {

    int fil = c[0] - 'a';
    int rank = c[1] - '1';
    return A1 + fil - 10 * rank;
}

int parse_square(int sq, const Color &color ) {

    int fil = sq%8;
    int rank = sq/8;
    int res=A1 + fil - 10 * rank;

    if (color == chess::Color::BLACK)
        return 119 - res;
    else
        return res;
}

int render_square(int sq, const Color & color)
{
    if (color == chess::Color::BLACK)
        sq = 119 - sq;

    int rank = (sq - A1) / 10;
    int fil = (sq - A1) % 10;

    if (fil < 0) {
        fil += 10;
        rank--;
    }


    return -rank * 8 + fil;

}
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPosition::CPosition()
{
    m_key=0;
    m_diservin_board=nullptr;
    m_score=0;
	//if (pos)
//		memcpy(&m_pos,pos, sizeof(POSI));


}


CPosition::~CPosition()
{
    if (m_diservin_board!=nullptr)
        delete m_diservin_board;




}


int CPosition::get_score() const
{
    return m_score;
}
int CPosition::get_player()
{
    return (int)m_diservin_board->sideToMove();
}
bool CPosition::has_major_pieces() const
{

    int c = m_diservin_board->sideToMove();
    return m_diservin_board->hasNonPawnMaterial(c);

}
int CPosition::board_end_state()
{
    std::pair resultado=m_diservin_board->isGameOver();
    return (int)(resultado.second); // WIN LOSE DRAW NONE
/*    if (resultado.first==GameResultReason::NONE)
        return 0;
    else{

        if (resultado.second==GameResult::DRAW)
            return 1;
        else
            return 2;
    }
*/

}
bool CPosition::is_dead()
{
    std::vector<Move> lst_moves;

    gen_moves(lst_moves);
    for (const Move& move : lst_moves) {
        if (move.score() >= MATE_LOWER)
                return true;
    }
    return false;

}
void CPosition::print_pos() const
{
    std::cout << "\nscore" <<  m_score;
    std::vector<Move> lst_moves;


    gen_moves(lst_moves);
    std::stable_sort(lst_moves.begin(), lst_moves.end(), orderMovesByScore);
    std::string suci;
    int i=0;
    for (int i=0; i < lst_moves.size(); i++) {
        suci = uci::moveToUci(lst_moves[i]);
        std::cout << "\n " << i << ") "  << suci << " score " << lst_moves[i].score();

    }


}

int CPosition::get_value_move(const Move &move) const
{

    Piece pieza= m_diservin_board->at(move.from());
    if (pieza !=Piece::underlying::NONE){
        int c=(int)pieza.color();
        int p=(int)pieza;
        if (p > 5)p-=6;
        int i = move.from().index();
        int j = move.to().index();

        //enroque añadir el cambio en los valores de las posiciones de las torres
        if (move.typeOf() == Move::CASTLING){
            if (j==7 || j==63){ // king side
                if (c == (int)chess::Color::WHITE)
                    return 48;
                else
                    return 56;
            }else{ // queen side
                if (c == (int)chess::Color::WHITE)
                    return 26;
                else
                    return 43;
            }
        }
        //Valor posicional de la pieza en la nueva y antigua posición
        int score = g_pst[c][p][j] - g_pst[c][p][i];
        // Captura añadir el valor de la casilla capturada (para esa pieza)
        Piece pieza2 = m_diservin_board->at(j);
        if (pieza2 !=Piece::underlying::NONE){
            // Si es una captura al paso, ajustamos la casilla capturada
            if(move.typeOf() == Move::ENPASSANT)
                score += g_pst[!c][0][j^8];
            else{
                int p2=(int)pieza2;
                if (p2 > 5)p2-=6;
                score += g_pst[!c][p2][j];
            }
        }

        if( move.typeOf() == Move::PROMOTION)
            score += g_pst[c][4][j] - g_pst[c][0][j];



        return score;
    }else
        return 0;


}

int CPosition::get_value_move2(const Move &move) const
{

    int j = move.to().index();

	if (move.typeOf() == Move::CASTLING){

        if (j==7 || j==63)
            return 56;
		else if(j==0 || j==56)
            return 43;

        //std::cout << "\n ENROQUE DESCONOCIDO???";
        return 0;
	}

    Piece pieza= m_diservin_board->at(move.from());
    if (pieza ==Piece::underlying::NONE){
            //std::cout << "\npieza jodida en get_value_move";
            return 0;
    }

    int i = move.from().index();
	int p_i=(int)pieza;
    if (p_i > 5)p_i-=6;


    Color color=m_diservin_board->sideToMove();

    int score = g_pst[color][p_i][j] - g_pst[color][p_i][i];

    if( move.typeOf() == Move::PROMOTION)
        score += g_pst[color][4][j] - g_pst[color][0][j];

    else if( move.typeOf() == Move::ENPASSANT){
        int sq_ep=j^8;
        Piece captured_piece=m_diservin_board->at(sq_ep);
        if (captured_piece !=Piece::underlying::NONE){
            score += g_pst[!color][0][sq_ep];
//            std::cout << "\nPASO " << uci::moveToUci(move) << " PIEZA " <<  captured_piece << " COLOR " << captured_color << " CASILLA " << sq_ep;
//            std::cout << "\n" << m_diservin_board->getFen();
        }

    }else{
        // Capture
        Piece captured_piece=m_diservin_board->at(j);
        if (captured_piece !=Piece::underlying::NONE){
            int p_j = (int)captured_piece;
            if (p_j > 5)
                p_j-=6;
            score += g_pst[!color][p_j][j];

        }

    }
    return score;
}

void CPosition::gen_moves(std::vector<Move> &lst_moves) const{

    /*NOTE COMENTEDD chess.hpp at line 3732 assert(checks <= 2)
    TO AVOID assert */

    if (m_diservin_board==nullptr){
        //std::cout << "\n TABLERO NULO";
        return;

    }

    Movelist lm;

    movegen::legalmoves(lm, *m_diservin_board);

    if (lm.size()<1) return;


    lst_moves.reserve(lm.size()); // Reserva espacio para evitar reasignaciones frecuentes

    int i=0;
    for (const auto &m : lm) {
        if( m.typeOf() == Move::PROMOTION && m.promotionType() != PieceType::underlying::QUEEN){
    //            std::cout << "\n" << uci::moveToUci(m) << " type " << m.typeOf() << " " << m.promotionType() ;
            continue;
        }
        lst_moves.push_back(m);
        lst_moves[i].setScore(get_value_move(m));
        i++;

    }

}


bool CPosition ::is_valid_board() const
{


    Square wksq = find_king_square(chess::Color::WHITE);
    if (wksq == Square::underlying::NO_SQ)
        return false;

    Square bksq = find_king_square(chess::Color::BLACK);
    if (bksq == Square::underlying::NO_SQ)
        return false;

    Color turn=m_diservin_board->sideToMove();
    if (turn == chess::Color::BLACK){
        if (m_diservin_board->isAttacked(wksq,chess::Color::BLACK))
            return false;
    }else{
        if (m_diservin_board->isAttacked(bksq,chess::Color::WHITE))
            return false;

    }

    return true;

}
bool CPosition ::is_valid_move(const Move& move)
{

    m_diservin_board->makeMove(move);
    bool valid= is_valid_board();
    m_diservin_board->unmakeMove(move);
    return valid;

}
void CPosition::undo_move(const Move& move,const std::uint64_t &old_key,const int old_score)
{

    m_diservin_board->unmakeMove(move);
    m_score = old_score;
    m_key=old_key;

}
void CPosition::make_move(const Move& move)
{


    m_diservin_board->makeMove(move);
    m_key = m_diservin_board->hash();
    m_score = -(m_score + move.score());

}

int CPosition::count_color_piezes(const Bitboard& bb)
{
    int contador = 0;
    for (int i = 0; i < 64; ++i) { // Itera sobre las 64 casillas del tablero
        if (bb.check(i)) { // Verifica si el bit correspondiente a la casilla 'i' está encendido
            contador++; // Incrementa el contador si la casilla está ocupada
        }
    }
    return contador;
}
int CPosition::count_board_piezes(const Board *board)
{
    Bitboard bb_us= board->us(board->sideToMove());
    Bitboard bb_them= board->them(board->sideToMove());
    int n1=CPosition::count_color_piezes(bb_us);
    int n2 =CPosition::count_color_piezes(bb_them);
    return n1+n2;

}
void CPosition::undo_nullmove(const std::uint64_t &old_key,const int old_score)
{

    m_diservin_board->unmakeNullMove();
    //Square ep= m_diservin_board->enpassantSq();

    m_score = old_score;
    m_key=old_key;

}

void CPosition::nullmove()
{

    m_diservin_board->makeNullMove();
    m_score = -m_score;
    m_key = m_diservin_board->hash();
}


void CPosition::simpleBoardEval()
{
    m_score=0;

    Color turn = m_diservin_board->sideToMove();

    for (int i=0; i <  64; i++){
        Piece piece = m_diservin_board->at(i); //.type();
        if (piece != Piece::underlying::NONE){
            int p=piece;
            if (p>5)p-=6;
            Color color=piece.color();
            if (color==chess::Color::WHITE)
                m_score += g_pst[color][p][i];
            else
                m_score -= g_pst[color][p][i];
        }

    }
    if (turn==chess::Color::BLACK)
        m_score = -m_score;

}
void CPosition::set_board(const Board & board)
{
    if (m_diservin_board!=nullptr)
        delete m_diservin_board;

    m_diservin_board = new Board();
    *m_diservin_board=board;

    simpleBoardEval();

    m_key = m_diservin_board->hash();


}

Square CPosition::find_king_square(const Color & color) const
{
    if (m_diservin_board->pieces(PieceType::KING, color) == Bitboard(0))
        return Square::underlying::NO_SQ;

    return m_diservin_board->kingSq(color);
}


bool CPosition::render_nnue_arrays(int *pieces , int *squares) const
{

    //nnue pieces
    //wking=1, wqueen=2, wrook=3, wbishop= 4, wknight= 5, wpawn= 6
    //bking=7, bqueen=8, brook=9, bbishop=10, bknight=11, bpawn=12

    pieces[0]=1;
    pieces[1]=7;


    squares[0]=find_king_square(chess::Color::WHITE).index();
    squares[1]= find_king_square(chess::Color::BLACK).index();


    int n=2;
    int nk=0;
    for (int i = 0; i < 64; i++) {

        Piece pieza= m_diservin_board->at(i);
        if (pieza !=Piece::underlying::NONE){

            int p = 6-(int)pieza.type();
            if (p != 1){ // if not a king
                if (pieza.color()==chess::Color::BLACK)
                    p+=6;
                pieces[n]=p;
                squares[n]=i;
                n++;
            }else{
                nk++;
            }


        }

    }


    pieces[n]=0;
    squares[n]=0;
    if (nk!=2) return false;
    else return true;


}
/*
std::string CPosition::render_number(int i)
{
    int rank = (i - A1) / 10;
    int fil = (i - A1) % 10;
    if (fil < 0) {
        fil += 10;
        rank--;
    }



}
std::string CPosition::render_pos()
{
    char color;
    if (get_color() == BLACK)
        color='b';
        rotate_self();
    else
        color='w';


}*/
