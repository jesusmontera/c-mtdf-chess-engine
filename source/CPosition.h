#ifndef CPOSITION_H
#define CPOSITION_H
#include <vector>
#include <string>
#include <chess.hpp>
using namespace chess;

// Position.h: interface for the CPosition class.
//
//////////////////////////////////////////////////////////////////////

const int MATE_LOWER = 50710;
const int MATE_UPPER = 69290;


bool orderMovesByScore(const Move& a, const Move& b);

class CPosition
{
public:

    //void render_nnue_arrays(int *pieces , int *squares) const;
    int get_score() const;
    bool is_dead();
    int get_player();
	bool has_major_pieces() const;
	void nullmove();
	bool is_valid_board() const;
	void print_pos() const;
	void undo_move(const Move &move,const std::uint64_t &old_key,const int old_score);
	void undo_nullmove(const std::uint64_t &old_key,const int old_score);
    Square find_king_square(const Color & color) const;
	void make_move(const Move& move);
    int get_value_move2(const Move &move) const;
	int get_value_move(const Move &move) const;
	void gen_moves(std::vector<Move> &lst_moves) const;
	CPosition();
    bool is_valid_move(const Move& move);
	virtual ~CPosition();
    std::uint64_t m_key;
    int m_score;
    Board *m_diservin_board;
    void set_board(const Board & board);
    int board_end_state();
    static int count_board_piezes(const Board *board);
    bool render_nnue_arrays(int *pieces , int *squares) const;
private:
    static int count_color_piezes(const Bitboard& bb);
    void simpleBoardEval();
	//POSI m_pos;









};


#endif // CPOSITION_H
