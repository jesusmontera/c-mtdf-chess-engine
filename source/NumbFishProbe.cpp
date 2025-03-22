#include "NumbFishProbe.h"
#include <time.h>
#include <iostream>
#include <chrono>
#include "nnue/nnue.h" // libprobe dll for nnue evaluation
#include "SyzygyOnline.h"


const int TABLE_SIZE = 1e7;
//const int EVAL_ROUGHNESS = 13;
//const int QS_LIMIT = 219;
const int EVAL_ROUGHNESS = 13;
const int QS_LIMIT = 219;

//char buf_key_table[150];




bool orderMovesByScore(const Move& a, const Move& b) {
    return a.score() > b.score();
}

NumbFishProbe::NumbFishProbe(bool use_nnue, bool use_book)
{
    //ctor

    //m_pbook= new Cbook("/home/luis/Documentos/codeblocks/libnumbfish/book/Perfect2021.bin");
    m_pbook= new Cbook("/home/luis/Documentos/codeblocks/libnumbfish/book/sourceforge/komodo.bin");
    m_claves_score_reusadas=0;

    m_use_nnue=use_nnue;
    if (m_use_nnue){
        nnue_init("/media/luis/48A0CF8FA0CF8244/luis/codigo phyton lubuntu/ajedrez/nnue/nn-04cf2b4ed1da.nnue");
    }
     m_nue_pieces= new int[33]; // allocate anyway only 33 int x2
     m_nue_squares= new int[33];
     m_use_book=use_book;

     bool mem_ok=true;
     /*try {
        m_tp_score.reserve(TABLE_SIZE/2);
    } catch (const std::bad_alloc& e){
        mem_ok=false;

    }*/
    const int TTS_SIZE=300000;
    int x=1;
    for (int d = 2; d < 11; d++){
        m_tp_score[0][d].reserve(TTS_SIZE/x);
        x++;
    }

    if (mem_ok){
        try {
            m_tp_move.reserve(TABLE_SIZE/2);
        } catch (const std::bad_alloc& e){
            mem_ok=false;

        }
    }

    if (!mem_ok)
        std::cerr << "# disservinfish WARNING no memory for tables" << std::endl;


     m_state=STATE_FREE;

}

NumbFishProbe::~NumbFishProbe()
{

    delete []m_nue_pieces;
    delete []m_nue_squares;
    delete m_pbook;


}
bool NumbFishProbe::valid_move_in_table(std::uint64_t &key)
{

    if (m_tp_move.find(key) == m_tp_move.end()){

        return false;
    }
    if (m_tp_move[key].has_value())
        return true;
    else return false;

}
/*
bool NumbFishProbe::set_gamma_move(const CPosition &obj_pos,
                                   int best,int gamma,
                                   const Move &move)
{

    if (best >= gamma){
        if (m_tp_move.size() > TABLE_SIZE)
            m_tp_move.clear();
        m_tp_move[obj_pos.m_key]=move; //std::nullopt;
        return true;
    }else
        return false;

}

bool NumbFishProbe::set_gamma_move_null(const CPosition &obj_pos,
                                   int best,int gamma)
{

    if (best >= gamma){
        if (m_tp_move.size() > TABLE_SIZE)
            m_tp_move.clear();
        m_tp_move[obj_pos.m_key]=std::nullopt;
        return true;
    }else
        return false;

}
*/

int NumbFishProbe::bound(CPosition &obj_pos, int gamma, int depth,bool root)
{
        if (m_time_out || m_state==STATE_STOP)
            return 0;
        m_nodes++;
        if (m_nodes%50000==0){
            m_time_out = is_time_out();
            if (m_time_out)
                return 0;
        }



        depth = std::max(depth, 0);
        if (obj_pos.get_score() <= -MATE_LOWER)
            return -MATE_UPPER;


        //if ( root == false && pos->board in self.history)
        //    return 0

       TABLE_ENTRY entry;

       //sprintf(buf_key_table,"%llu_%d_%d",obj_pos.m_key,depth, root);
       //std::string key_table=buf_key_table;
        //KEY_TABLE_SCORE key_table={obj_pos.m_key,depth, root};

        std::unordered_map<std::uint64_t, TABLE_ENTRY>* p_tp_score = &m_tp_score[root][depth];

        if (p_tp_score->find(obj_pos.m_key) == p_tp_score->end())
            entry={-MATE_UPPER, MATE_UPPER};
        else{
            entry=(*p_tp_score)[obj_pos.m_key]; //.value();
            m_claves_score_reusadas++;
        }

        if (entry.lower >= gamma && (root==false || valid_move_in_table(obj_pos.m_key) ))
            return entry.lower;
        if (entry.upper < gamma)
            return entry.upper;

        std::uint64_t old_key=obj_pos.m_key; // save for recursion
        int old_score=obj_pos.m_score;

        // Run through the moves, shortcutting when possible
        int best = -MATE_UPPER;
        bool gamma_break=false;

        // First try not moving at all. We only do this if there is at least one major
        //piece left on the board, since otherwise zugzwangs are too dangerous.
        if (depth > 0 && root == false && obj_pos.has_major_pieces()){
            obj_pos.nullmove();
            int score;
            if (obj_pos.is_valid_board())
                score = -bound(obj_pos, 1-gamma, depth-3, false);
            else
                score= - MATE_UPPER;
            obj_pos.undo_nullmove(old_key,old_score);
            best = std::max(best, score);
            if (best >= gamma){
                if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                m_tp_move[obj_pos.m_key]=std::nullopt;
                gamma_break=true;
            }
        }

        if (depth == 0 && gamma_break==false){
            int score;
            if( ! m_use_nnue)
                score = obj_pos.get_score();
            else{
                //std::string fen= obj_pos.m_diservin_board->getFen();
                //score = nnue_evaluate_fen(fen.c_str());
                int player=obj_pos.get_player();
                if (obj_pos.render_nnue_arrays(m_nue_pieces , m_nue_squares))
                    score= nnue_evaluate(player,m_nue_pieces,m_nue_squares);
                else
                    score = obj_pos.get_score();


            }
            best = std::max(best, score);
            if (best >= gamma){
                if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                m_tp_move[obj_pos.m_key]=std::nullopt;
                gamma_break=true;
            }
        }

        if (gamma_break==false){
            if (valid_move_in_table(obj_pos.m_key)){
                Move killer = m_tp_move[obj_pos.m_key].value();
                int score=0;
                if (depth > 0 || killer.score() >= QS_LIMIT){

                    if (obj_pos.is_valid_move(killer))
                    {
                        obj_pos.make_move(killer);
                        score = -bound(obj_pos, 1-gamma, depth-1, false);
                        obj_pos.undo_move(killer,old_key,old_score);
                    }else
                        score=MATE_UPPER;
                    best = std::max(best, score);
                    if (best >= gamma){
                        if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                        m_tp_move[obj_pos.m_key] = killer;
                        gamma_break=true;
                    }
                }
            }
        }

        //Then all the other moves

        std::vector<Move> lst_moves;
        lst_moves.reserve(40);
        if (gamma_break==false){
            obj_pos.gen_moves(lst_moves);

            // Ordenar la lista de mayor a menor por el campo value
            std::stable_sort(lst_moves.begin(), lst_moves.end(), orderMovesByScore);
            //   TESTING REDUCCION DE MOVIMIENTOS
           // int num_piezes= CPosition::count_board_piezes(obj_pos.m_diservin_board);

             for (const Move& move : lst_moves) {
                if (depth > 0 || move.score() >= QS_LIMIT){

                    int score;
                    if (obj_pos.is_valid_move(move)){
                        obj_pos.make_move(move);
                        score= -bound(obj_pos, 1-gamma, depth-1, false);
                        obj_pos.undo_move(move,old_key,old_score);

                    }else
                        score=MATE_UPPER;

                    best = std::max(best, score);
                    if (best >= gamma){
                        if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                        m_tp_move[obj_pos.m_key] = move;
                        break;
                    }


                }
             }
        }
        if (best < gamma && best < 0 && depth > 0){
            int state= obj_pos.board_end_state();
            if (state==2) // DRAW
                best = 0;
            else if (state == 1) // LOSE
                best =-MATE_UPPER;

        }

        // Clear before setting, so we always have a value
        //if (m_tp_score.size() > TABLE_SIZE) m_tp_score.clear();
        //Table part 2

        if (best >= gamma)
            (*p_tp_score)[obj_pos.m_key] = {best, entry.upper};
        if (best < gamma)
            (*p_tp_score)[obj_pos.m_key] = {entry.lower, best};


        return best;


}

bool NumbFishProbe::is_time_out()
{
    clock_t t_end = clock();
    double elapsed = double(t_end - m_time_start)/CLOCKS_PER_SEC;
    if (elapsed >= m_move_time)
        return true;
    return false;
}

void NumbFishProbe::mtdf_gamma(CPosition &pos, int depth)
{
        //int aspiration=(int)(MATE_UPPER/100.)*(depth);

        int lower=-MATE_UPPER;
        int upper = MATE_UPPER;
        int gamma=0,score=0;
        while (lower < upper - EVAL_ROUGHNESS){
            gamma = (lower+upper+1)/2;
            score = bound(pos, gamma, depth,true);

            m_time_out = is_time_out();
            if (m_time_out || m_state==STATE_STOP)
                return;

            if (score >= gamma)
                lower = score;
            if (score < gamma)
                upper = score;


        }
        bound(pos, lower, depth,true);


}
void info_to_file(const char *s)
{
    FILE *fp=fopen("/media/luis/48A0CF8FA0CF8244/luis/codigo phyton lubuntu/ajedrez/torneos/prueba.txt","a");

    fprintf(fp,"%s\n",s);
    fclose(fp);
}
int random_number(int a, int b) {
  // Semilla para el generador de números aleatorios
  //
  return a + std::rand() % (b - a + 1);
}
void NumbFishProbe::avoid_repetitions(const Board &board, std::vector<MOVE_SCORE_DEPTH> &msd )
{


    int n = msd.size();
    if (n < 6) return;
    std::srand(std::time(0));
    Board bb = board;

    for (int i=0; i < n; i++){
        int r = random_number(5,n-1);
        if (msd[r].score > 100){ //&& m_score -msd[r].score < 100 ){
            bb.makeMove(msd[i].move);
            if (!bb.isRepetition(3)){
                m_result_move=uci::moveToUci(msd[r].move);
                m_depth = msd[r].depth;
                m_score = msd[r].score;
                return;

            }
            bb.unmakeMove(msd[i].move);

        }
    }



}
bool NumbFishProbe::move_lichess_syzygy(const Board &board)
{

    int num_piezes= CPosition::count_board_piezes(&board);
    if (num_piezes <8){
        SyzygyOnline obj_query;
        Move move_syzygy;
        bool query_ok= obj_query.query_lichess_syzygy(board, move_syzygy);
        if (query_ok){
            m_result_move = uci::moveToUci(move_syzygy);
            m_state = STATE_END_SEARCH;
            return true;
        }else
            std::cerr << "# Syzyg QUERY FAILED " << std::endl;

    }

    return false;
}
bool NumbFishProbe::move_opening_book(const Board &board)
{
    if (m_use_book){
        std::string uci_book_move = m_pbook->get_random_book_move(board.getFen());
        if (uci_book_move !=""){
            m_result_move = uci_book_move;
            m_state = STATE_END_SEARCH;
            return true;

        }else
            m_use_book=false;
    }
    return false;

}
void NumbFishProbe::clear_search_variables()
{
    m_score=0;
    m_nodes=0;
    m_depth=0;
    m_result_move="";
    for (int i = 0; i < 2; ++i) {
        for (int j = 1; j < 101; ++j) {
            m_tp_score[i][j].clear();

        }
    }
    m_tp_move.clear();



}
//void NumbFishProbe::get_move(const std::string &fen, double secs)
void NumbFishProbe::get_move(const Board &board, double secs,bool bprintinfo)
{


    m_state = STATE_SEARCHING;



    if (move_lichess_syzygy(board))
        return;
    if (move_opening_book(board))
        return;

    clear_search_variables();
    CPosition pos;
    //pos.set_fen(fen.c_str());
    pos.set_board(board);
    if( m_use_nnue){
        int player  = pos.get_player();

        if (pos.render_nnue_arrays(m_nue_pieces , m_nue_squares))
            pos.m_score = nnue_evaluate(player,m_nue_pieces,m_nue_squares);
    }

    //std::cout << "\nINITIAL SCORE " << pos.get_score();
    int depth;

    m_time_out=false;
    m_move_time=secs;
    m_time_start = clock();


    std::vector<MOVE_SCORE_DEPTH> msd;

    for (depth = 1;  depth<100; depth++){
        mtdf_gamma(pos,depth);
        if (m_time_out || m_state==STATE_STOP) break;

        Move m=m_tp_move[pos.m_key].value();

        m_result_move = uci::moveToUci(m);

        //sprintf(buf_key_table,"%llu_%d_%d",pos.m_key,depth, true);
        //std::string key_table=buf_key_table;
        //KEY_TABLE_SCORE key_table={pos.m_key,depth, true};
        if (m_tp_score[true][depth].find(pos.m_key) != m_tp_score[true][depth].end()){
            m_score = m_tp_score[true][depth][pos.m_key].lower;
            msd.push_back({m,m_score,depth});

        }else{
            msd.push_back({m,0,depth});
        }


        if (bprintinfo){
           std::cout << "\ndepth " << depth << " " << m_result_move << " score " << m_score << " nodes " << m_nodes;
        }

        if (is_time_out() > secs || m_score >=MATE_UPPER)
            break;

    }
    m_depth=depth;
    if (board.isRepetition(2))
        avoid_repetitions(board,msd);

    clock_t t_end = clock();
    double elapsed = double(t_end - m_time_start)/CLOCKS_PER_SEC;

    m_state = STATE_END_SEARCH;

}
