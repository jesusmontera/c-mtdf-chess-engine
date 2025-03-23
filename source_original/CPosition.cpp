// Position.cpp: implementation of the CPosition class.
//
//////////////////////////////////////////////////////////////////////

#include "CPosition.h"
#include  <ctype.h>
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream> // para cout

#define MAX_MOVES 100

#define N -10
#define E 1
#define S 10
#define W  -1
#define NUM_PIECES  6 // Number of piece types
#define MAX_DIRECTIONS  8 // Maximum number of directions for a piece

#define A1 91
#define H1 98
#define A8 21
#define H8 28

const int WHITE = 0;
const int BLACK = 1;

char m_key_buf[150];

const char LETRAS_PIEZAS[7]="PNBRQK";
const char LETRAS_NNUE[8]=" KQRBNP";
const int DIRECTIONS[NUM_PIECES][MAX_DIRECTIONS] = {
    // Pawn
    {N, N + N, N + W, N + E},
    // Knight (implement knight moves here)
    {N + N + E, E + N + E, E + S + E, S + S + E, S + S + W, W + S + W, W + N + W, N + N + W},
    // Bishop
    {N + E, S + E, S + W, N + W},
    // Rook
    {N, E, S, W},
    // Queen
    {N, E, S, W, N + E, S + E, S + W, N + W},
    // King
    {N, E, S, W, N + E, S + E, S + W, N + W}
};
const int NUM_DIRECTIONS[NUM_PIECES]={4,8,4,4,8,8};
const int PST[NUM_PIECES][NUM_LETTERS_BOARD] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 178, 183, 186, 173, 202, 182, 185, 190, 0, 0, 107, 129, 121, 144, 140, 131, 144, 107, 0, 0, 83, 116, 98, 115, 114, 100, 115, 87, 0, 0, 74, 103, 110, 109, 106, 101, 100, 77, 0, 0, 78, 109, 105, 89, 90, 98, 103, 81, 0, 0, 69, 108, 93, 63, 64, 86, 103, 69, 0, 0, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 214, 227, 205, 205, 270, 225, 222, 210, 0, 0, 277, 274, 380, 244, 284, 342, 276, 266, 0, 0, 290, 347, 281, 354, 353, 307, 342, 278, 0, 0, 304, 304, 325, 317, 313, 321, 305, 297, 0, 0, 279, 285, 311, 301, 302, 315, 282, 280, 0, 0, 262, 290, 293, 302, 298, 295, 291, 266, 0, 0, 257, 265, 282, 280, 282, 280, 257, 260, 0, 0, 206, 257, 254, 256, 261, 245, 258, 211, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 261, 242, 238, 244, 297, 213, 283, 270, 0, 0, 309, 340, 355, 278, 281, 351, 322, 298, 0, 0, 311, 359, 288, 361, 372, 310, 348, 306, 0, 0, 345, 337, 340, 354, 346, 345, 335, 330, 0, 0, 333, 330, 337, 343, 337, 336, 320, 327, 0, 0, 334, 345, 344, 335, 328, 345, 340, 335, 0, 0, 339, 340, 331, 326, 327, 326, 340, 336, 0, 0, 313, 322, 305, 308, 306, 305, 310, 310, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 514, 508, 512, 483, 516, 512, 535, 529, 0, 0, 534, 508, 535, 546, 534, 541, 513, 539, 0, 0, 498, 514, 507, 512, 524, 506, 504, 494, 0, 0, 479, 484, 495, 492, 497, 475, 470, 473, 0, 0, 451, 444, 463, 458, 466, 450, 433, 449, 0, 0, 437, 451, 437, 454, 454, 444, 453, 433, 0, 0, 426, 441, 448, 453, 450, 436, 435, 426, 0, 0, 449, 455, 461, 484, 477, 461, 448, 447, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 935, 930, 921, 825, 998, 953, 1017, 955, 0, 0, 943, 961, 989, 919, 949, 1005, 986, 953, 0, 0, 927, 972, 961, 989, 1001, 992, 972, 931, 0, 0, 930, 913, 951, 946, 954, 949, 916, 923, 0, 0, 915, 914, 927, 924, 928, 919, 909, 907, 0, 0, 899, 923, 916, 918, 913, 918, 913, 902, 0, 0, 893, 911, 929, 910, 914, 914, 908, 891, 0, 0, 890, 899, 898, 916, 898, 893, 895, 887, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60004, 60054, 60047, 59901, 59901, 60060, 60083, 59938, 0, 0, 59968, 60010, 60055, 60056, 60056, 60055, 60010, 60003, 0, 0, 59938, 60012, 59943, 60044, 59933, 60028, 60037, 59969, 0, 0, 59945, 60050, 60011, 59996, 59981, 60013, 60000, 59951, 0, 0, 59945, 59957, 59948, 59972, 59949, 59953, 59992, 59950, 0, 0, 59953, 59958, 59957, 59921, 59936, 59968, 59971, 59968, 0, 0, 59996, 60003, 59986, 59950, 59943, 59982, 60013, 60004, 0, 0, 60017, 60030, 59997, 59986, 60006, 59999, 60040, 60018, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPosition::CPosition()
{
    m_key[0]='\0';
	//if (pos)
//		memcpy(&m_pos,pos, sizeof(POSI));


}


CPosition::~CPosition()
{




}

inline int get_index_letter(const char l)
{
	for (int i=0; i < 6; i++){
		if (l == LETRAS_PIEZAS[i])
			return i;
	}
	return -1;
}
int CPosition::get_score() const
{
    return m_pos.score;
}
bool CPosition::has_major_pieces() const
{

    char c;
    int i=0;
    while (i <NUM_LETTERS_BOARD){
        c = m_pos.board[i];
        if (c=='R' || c=='B' || c=='N' || c=='Q')
            return true;
        i++;
    }
    return false;

}
bool CPosition::is_dead() const
{
    std::vector<MOVE> lst_moves;

    gen_moves(lst_moves);
    for (const MOVE& move : lst_moves) {
        if (move.value >= MATE_LOWER)
                return true;
    }
    return false;

}
int CPosition::get_value_move(const MOVE &move) const
{

	int i = move.from;
	int j = move.to;
    char p = m_pos.board[i];
	char q = m_pos.board[j];

    // Actual move
    int score = PST[get_index_letter(p)][j] - PST[get_index_letter(p)][i];
    // Capture
    if (islower(q))
        score += PST[get_index_letter(toupper(q))][119-j];
    // Castling check detection
    if (abs(j - m_pos.kp) < 2)
        score += PST[get_index_letter('K')][119-j];
    // Castling
    if (p == 'K' && abs(i-j) == 2){
        score += PST[get_index_letter('R')][(i+j)/2];   //2];
		if (j < i)
			score -= PST[get_index_letter('R')][A1];
		else
			score -= PST[get_index_letter('R')][H1];
	}
    // Special pawn stuff
    if (p == 'P'){
        if (A8 <= j && j <= H8)
            score += PST[get_index_letter('Q')][j] - PST[get_index_letter('P')][j];
        if (j == m_pos.ep)
            score += PST[get_index_letter('P')][119-(j+S)];
	}
    return score;
}
int CPosition::gen_arr_moves(MOVE *arr_moves)
{
    const char *board=(const char *) m_pos.board;
    int num_moves=0;
	for (int i=0; i < NUM_LETTERS_BOARD;i++){
		char p=board[i];

		if ( ! isupper(p)) continue;
		int p_index=get_index_letter(p);
		for( int nd=0; nd <  NUM_DIRECTIONS[p_index];nd++){
			int d=DIRECTIONS[p_index][nd];
			int j = i + d;
			while(1){
				char q = board[j];
				if(isspace(q) || isupper(q)) break;
				if (p == 'P' && (d==N || d==N+N) && q != '.') break;
                if (p == 'P' && d == N+N && (i < A1+N || board[i+N] != '.')) break;
                if (p == 'P' && (d == N+W || d== N+E) && q == '.' &&
                    (j!=m_pos.ep  && (j < m_pos.kp-1 || j >m_pos.kp+1))) break;




				arr_moves[num_moves++]={i,j,0};
				if (p =='P' || p =='N' || p =='K' || islower(q)) break;

				if (i == A1 && board[j+E] == 'K' && m_pos.wc[0]) {
					arr_moves[num_moves++]={j+E,j+W,0};

				}
				if (i == H1 && board[j+W] == 'K' && m_pos.wc[1]) {
					arr_moves[num_moves++] = {j+W, j+E,0};
				}

				j += d;
			}
		}

	}
	for (int i=0; i < num_moves; i++)
        arr_moves[i].value = get_value_move(arr_moves[i]);
    return num_moves;

}
void CPosition::gen_moves(std::vector<MOVE> &lst_moves) const
{

	const char *board=(const char *) m_pos.board;
	lst_moves.clear();
	for (int i=0; i < NUM_LETTERS_BOARD;i++){
		char p=board[i];

		if ( ! isupper(p)) continue;
		int p_index=get_index_letter(p);
		for( int nd=0; nd <  NUM_DIRECTIONS[p_index];nd++){
			int d=DIRECTIONS[p_index][nd];
			int j = i + d;
			while(1){
				char q = board[j];
				if(isspace(q) || isupper(q)) break;
				if (p == 'P' && (d==N || d==N+N) && q != '.') break;
                if (p == 'P' && d == N+N && (i < A1+N || board[i+N] != '.')) break;
                if (p == 'P' && (d == N+W || d== N+E) && q == '.' &&
                    (j!=m_pos.ep  && (j < m_pos.kp-1 || j >m_pos.kp+1))) break;


				MOVE move = {i,j,0};
				move.value = get_value_move(move);
				lst_moves.push_back(move);
				if (p =='P' || p =='N' || p =='K' || islower(q)) break;

				if (i == A1 && board[j+E] == 'K' && m_pos.wc[0]) {
					MOVE move = {j+E,j+W,0};
					move.value = get_value_move(move);
					lst_moves.push_back(move);
				}
				if (i == H1 && board[j+W] == 'K' && m_pos.wc[1]) {
					MOVE move = {j+W, j+E,0};
					move.value = get_value_move(move);
					lst_moves.push_back(move);
				}

				j += d;
			}
		}

	}

}


/*void CPosition::set_pos(const char *board)
{
	int i;
//	strncpy(pos->board,board,NUM_LETTERS_BOARD);
	for (i=0; i < NUM_LETTERS_BOARD; i++){
		m_pos.board[i]=board[i];
	}
	for (i=0; i < 2; i++){
		m_pos.wc[i]=false;
		m_pos.bc[i]=false;
	}
	m_pos.ep =0;
	m_pos.kp =0;
	m_pos.score=0;

}*/
/*
bool compararPorValor(const MOVE& a, const MOVE& b) {
    return a.value > b.value;
}
/ Ordenar el vector por el campo 'value' de forma descendente
    std::sort(lst_moves.begin(), lst_moves.end(), compararPorValor);
*/
CPosition CPosition::make_move(const MOVE& move) const
{
    int i = move.from;
    int j = move.to;
	//CPosition obj_pos = CPosition(&m_pos);
	CPosition obj_pos;


	POSI *pos = obj_pos.get_posi_struct();
    memcpy(pos,&m_pos, sizeof(POSI));


	pos->ep=0;
	pos->kp=0;
	//pos->score = m_pos.score + get_value_move(move);
	pos->score = m_pos.score + move.value;

    char p = m_pos.board[i];
   // char q = m_pos.board[j];

	// Actual move
    pos->board[j] = pos->board[i];
	pos->board[i] = '.';
    // Castling rights, we move the rook or capture the opponent's
    if (i == A1)
		pos->wc[0] = false;
    else if (i == H1)
		pos->wc[1] = false;

    if (j == A8)
		pos->bc[1] = false;
    else if (j == H8)
		pos->bc[0] = false;

	 // Castling
    if (p == 'K'){
        pos->wc[0] = false;
		pos->wc[1] = false;

        if (abs(j-i) == 2){
            pos->kp = (i+j)/2;
			if (j < i)
				pos->board[A1]='.';
			else
				pos->board[H1]='.';

            pos->board[pos->kp]='R';
		}
	}
	  // Pawn promotion, double move and en passant capture
    if (p == 'P'){
        if (A8 <= j && j <= H8)
            pos->board[j]= 'Q';
        if (j - i == 2*N)
            pos->ep = i + N;
        if (j == m_pos.ep)
            pos->board[ j+S ] = '.';
	}


	obj_pos.rotate_self();
	return obj_pos; // FALTA ROTAR

}

POSI * CPosition::get_posi_struct()
{
	return &m_pos;

}

void CPosition::rotate_self()
{
	int i;
	POSI posi_rot;
	//CPosition obj_pos;

	posi_rot.score = -m_pos.score;
	if (m_pos.ep)
		posi_rot.ep = 119-m_pos.ep;
	else
		posi_rot.ep=0;

	if (m_pos.kp)
		posi_rot.kp = 119-m_pos.kp;
	else
		posi_rot.kp =0;
	for (i=0; i < 2; i++){
		posi_rot.bc[i]= m_pos.bc[i];
		posi_rot.wc[i]= m_pos.wc[i];

	}
	int x=NUM_LETTERS_BOARD -1;

	for (i=0; i < NUM_LETTERS_BOARD;i++){
		if( isalpha ( m_pos.board[i]))
			posi_rot.board[x] = m_pos.board[i] ^ 32;
		else
			posi_rot.board[x]=  m_pos.board[i];
		x--;

	}
	memcpy(&m_pos,&posi_rot, sizeof(POSI));

}

CPosition CPosition::nullmove() const
{

    // like rotate but clears ep and kp
    int i;
    CPosition obj_pos;
    POSI *pos = obj_pos.get_posi_struct();

    int x=NUM_LETTERS_BOARD -1;

	for (i=0; i < NUM_LETTERS_BOARD;i++){
		if( isalpha ( m_pos.board[i]))
			pos->board[x] = m_pos.board[i] ^ 32;
		else
			pos->board[x]=  m_pos.board[i];
		x--;

	}
	pos->score= -m_pos.score;
	pos->ep=0;
	pos->kp=0;
	for (i=0; i < 2; i++){
		pos->bc[i]= m_pos.bc[i];
		pos->wc[i]= m_pos.wc[i];

	}

    return obj_pos;
}



int parse(char c[2]) {
    int fil = c[0] - 'a';
    int rank = c[1] - '1';
    return A1 + fil - 10 * rank;
}

void CPosition::parseFEN(const char *fen) {
    char board[256] = {0};
    char color[2], castling[5], enpas[3];
    int hclock, fclock;

    sscanf(fen, "%s %s %s %s %d %d", board, color, castling, enpas, &hclock, &fclock);


    for (int i=0; i < NUM_LETTERS_BOARD;i++){
        m_pos.board[i] = ' ';
    }
    // Replace digits with dots
    int j = 21;
    for (int i = 0; board[i] != '\0'; i++) {
        if (isdigit(board[i])) {
            int num = board[i] - '0';
            for (int k = 0; k < num; k++) {
                m_pos.board[j++] = '.';
            }
        } else if (board[i]=='/')
            j+=2;
        else
            m_pos.board[j++] = board[i];

    }

    // Insert newlines
    for (int i = 9; i < NUM_LETTERS_BOARD; i += 10) {
        if (i < NUM_LETTERS_BOARD)
            m_pos.board[i] = '\n';
    }


    m_pos.wc[0]=false; m_pos.wc[1]=false;
    m_pos.bc[0]=false; m_pos.bc[1]=false;
    m_pos.kp=0;
    // Check castling rights
    if (strchr(castling, 'Q')!= NULL)
        m_pos.wc[0]=true;
    if (strchr(castling, 'K')!= NULL)
        m_pos.wc[1]=true;

    if (strchr(castling, 'k')!= NULL)
        m_pos.bc[0]=true;
    if (strchr(castling, 'q')!= NULL)
        m_pos.bc[1]=true;

    m_pos.ep = (strcmp(enpas, "-") != 0) ? parse(enpas) : 0;


    //char python_board[]="         \n         \n ....k...\n ....P...\n .....K..\n ........\n ........\n ........\n ........\n ........\n         \n         \n";

    int score=0;
    for (int i=0; i < NUM_LETTERS_BOARD;i++){
        char c=m_pos.board[i];
        m_pos.board[i]=c;
        if( isalpha (c) ){
            if(isupper(c)){
                score+=PST[get_index_letter(c)][i];
            }else if (islower(c)){

                score-=PST[get_index_letter(toupper(c))][119-i];
            }

        }
    }
    m_pos.score=score;

    if (color[0] =='b')
        rotate_self();



    /*std::cout << m_pos.board;
    std::cout << "\nscore " << m_pos.score;
    std::cout << "\nWHITE KING CASTLING " << m_pos.wc[1];
    std::cout << "\nWHITE QUEEN CASTLING " << m_pos.wc[0];
    std::cout << "\nBLACK KING CASTLING " << m_pos.bc[0];
    std::cout << "\nBLACK QUEEN CASTLING " << m_pos.bc[1];
    std::cout << "\nEP " << m_pos.ep;
    std::cout << "\n color " << color;
*/
    // You can now use formattedBoard, wc, bc, and ep as needed
}
void CPosition::make_key()
{
    sprintf(m_key_buf,"%.120s_%d_%d_%d_%d_%d_%d",
            m_pos.board,m_pos.wc[0], m_pos.wc[1],
            m_pos.bc[0], m_pos.bc[1],
            m_pos.ep, m_pos.kp);
     m_key = m_key_buf;

}
void CPosition::make_key2()
{

    int k;

    for (k = 0; k < NUM_LETTERS_BOARD; k++) {
        if( isalpha(m_pos.board[k]))
            break;
    }

    m_key="";
    int x=0;
    int v;
    for (int i = k; i < NUM_LETTERS_BOARD; i++) {

        if( !isalpha(m_pos.board[i])) continue;

        m_key_buf[x++] = m_pos.board[i];
        v=i-k;
        sprintf(m_key_buf+x,"%d",v);
        if (v >99)
            x+=3;
        else if (v >9)
            x+=2;
        else x++;

        //m_key += std::to_string(i); // Añadimos el índice directamente
    }

    sprintf(m_key_buf+x,"_%d_%d_%d_%d_%d_%d",
            m_pos.wc[0], m_pos.wc[1],
            m_pos.bc[0], m_pos.bc[1],
            m_pos.ep, m_pos.kp);
    m_key =m_key_buf;

}
int CPosition::get_color() const {
    return m_pos.board[0]=='\n' ? BLACK : WHITE;
}

std::string CPosition::move_render(const MOVE& move) {

    char p;
    int m[2]={move.from, move.to};



    if (A8 <= m[1] && m[1] <= H8 && m_pos.board[m[0]] == 'P') {
        p = 'q';
    } else {
        p = '\0';
    }

    if (get_color() == BLACK) {
        m[0] = 119 - m[0];
        m[1] = 119 - m[1];
    }

    int rank1 = (m[0] - A1) / 10;
    int fil1 = (m[0] - A1) % 10;
    int rank2 = (m[1] - A1) / 10;
    int fil2 = (m[1] - A1) % 10;

    if (fil1 < 0) {
        fil1 += 10;
        rank1--;
    }
    if (fil2 < 0) {
        fil2 += 10;
        rank2--;
    }

    char buf[10];
    sprintf(buf, "%c%d%c%d%c",
            fil1 + 'a',
            -rank1 + 1,
            fil2 + 'a',
            -rank2 + 1, p);
    std::string result = buf;
    return result;
}

void CPosition::render_nnue_arrays(int *pieces , int *squares) const
{
    char c,c_up;
    int row=7;
    int col=0;
    int p,sq;
    //nnue pieces
    //wking=1, wqueen=2, wrook=3, wbishop= 4, wknight= 5, wpawn= 6
    //bking=7, bqueen=8, brook=9, bbishop=10, bknight=11, bpawn=12
    int idx=2;
    for (int i = 0; i < NUM_LETTERS_BOARD; i++) {

        c=m_pos.board[i];
        if (c =='.'){
            col++;
            if (col==8){
                col=0;
                row--;
            }
        }

        if( !isalpha(c)) continue;


        c_up=toupper(c);
        for (p=1; p <7;p++){
            if (c_up == LETRAS_NNUE[p])
                break;
        }

        if (c_up=='K'){
            if (islower(c)) { // black piece
                pieces[1]=p+6;
                squares[1]=row*8+col;
            }else{
                pieces[0]=p;
                squares[0]=row*8+col;
            }

        }else{
            if (islower(c))// black piece
                p+=6;
            pieces[idx]=p;
            squares[idx]=row*8+col;
            idx++;
        }

        col++;
        if (col==8){
            col=0;
            row--;
        }


    }
    pieces[idx]=0;
    squares[idx]=0;


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
