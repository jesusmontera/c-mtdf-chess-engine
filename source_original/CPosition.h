#ifndef CPOSITION_H
#define CPOSITION_H
#include <vector>
#include <string>
// Position.h: interface for the CPosition class.
//
//////////////////////////////////////////////////////////////////////

const int MATE_LOWER = 50710;
const int MATE_UPPER = 69290;

#define NUM_LETTERS_BOARD 120
struct MOVE
{
	int from;
	int to;
	int value;
};
struct POSI{
	char board [NUM_LETTERS_BOARD];
	int score;
	bool wc[2];
	bool bc[2];
	int ep;
	int kp;
};

class CPosition
{
public:
    const char * get_board()const {
        return (const char * ) m_pos.board;
    }
    void make_key();
    void make_key2();
    std::string move_render(const MOVE& m);
    int gen_arr_moves(MOVE *arr_moves);
    int get_color() const;
    void render_nnue_arrays(int *pieces , int *squares) const;
    int get_score() const;
    bool is_dead() const;
	void rotate_self();
	bool has_major_pieces() const;
	POSI * get_posi_struct();
	CPosition nullmove() const;
	CPosition make_move(const MOVE& move) const;
	//void set_pos(const char *board);
	int get_value_move(const MOVE &move) const;
	void gen_moves(std::vector<MOVE> &lst_moves) const;
	CPosition();

	virtual ~CPosition();
    void parseFEN(const char *fen);
    std::string m_key;

private:
	POSI m_pos;







};


#endif // CPOSITION_H
