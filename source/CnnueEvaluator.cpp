#include "CnnueEvaluator.h"
#include "nnue/nnue.h"
//#include "CPosition.h"
CnnueEvaluator::CnnueEvaluator()
{
    m_pieces= new int[33];
    m_squares= new int[33];

    nnue_init("/media/luis/48A0CF8FA0CF8244/luis/codigo phyton lubuntu/ajedrez/nnue/nn-04cf2b4ed1da.nnue");
}

CnnueEvaluator::~CnnueEvaluator()
{
    delete []m_pieces;
    delete []m_squares;

}



int CnnueEvaluator::get_nnue_board_value(const char *board)
{
    /*self.nue_pieces[0]=1 # white king piece in nnue
        self.nue_squares[0]= board.king(chess.WHITE)
        self.nue_pieces[1]=7 # black king piece in nnue
        self.nue_squares[1]= board.king(chess.BLACK)
    */
    //int player =CPosition::render_nnue_arrays(board,m_pieces,m_squares);
    int value=0;
    return value;
   /* for (int i=0; i < 120; i++)
    {

        if (board[i]=='K'){
            m_pieces[0]=1; // white king
            m_squares[0]= CPosition.render_square(i);

        }

    }*/

}
