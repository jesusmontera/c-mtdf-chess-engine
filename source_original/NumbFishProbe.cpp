#include "NumbFishProbe.h"
#include <time.h>
#include <iostream>
#include <chrono>
#include "nnue/nnue.h" // libprobe dll for nnue evaluation
const int EVAL_ROUGHNESS = 13;
const int TABLE_SIZE = 1e7;
const int QS_LIMIT = 219;
const int MAX_MOVES_MEMORY= 50000;
char buf_key_table[20];

double g_time1=0;
double g_time2=0;

bool orderMovesByValue(const MOVE& a, const MOVE& b) {
    return a.value > b.value;
}

NumbFishProbe::NumbFishProbe(bool use_nnue, bool use_book)
{
    //ctor
    m_state=STATE_FREE;
    //m_pbook= new Cbook("/home/luis/Documentos/codeblocks/libnumbfish/book/Perfect2021.bin");
    m_pbook= new Cbook("/home/luis/Documentos/codeblocks/libnumbfish/book/sourceforge/komodo.bin");
    m_claves_score_reusadas=0;
    arr_moves= new MOVE[MAX_MOVES_MEMORY + 150];
    m_use_nnue=use_nnue;
    if (m_use_nnue){
        nnue_init("/media/luis/48A0CF8FA0CF8244/luis/codigo phyton lubuntu/ajedrez/nnue/nn-04cf2b4ed1da.nnue");
    }
     m_nue_pieces= new int[33]; // allocate anyway only 33 int x2
     m_nue_squares= new int[33];
     m_use_book=use_book;


}

NumbFishProbe::~NumbFishProbe()
{
    delete []arr_moves;
    delete []m_nue_pieces;
    delete []m_nue_squares;
    delete m_pbook;


}
bool NumbFishProbe::valid_move_in_table(std::string &key)
{

    if (m_tp_move.find(key) == m_tp_move.end()){

        return false;
    }
    if (m_tp_move[key].has_value())
        return true;
    else return false;

}
int NumbFishProbe::bound(CPosition &obj_pos, int gamma, int depth,bool root)
{
        m_nodes++;

        depth = std::max(depth, 0);
        //POSI *pos = obj_pos.get_posi_struct();
        if (obj_pos.get_score() <= -MATE_LOWER)
            return -MATE_UPPER;

        if (obj_pos.m_key[0]=='\0'){

            obj_pos.make_key();

        }

        //if ( root == false && pos->board in self.history)
        //    return 0

        /* Look in the table if we have already searched this position before.
         We also need to be sure, that the stored search was over the same
         nodes as the current search. */
        TABLE_ENTRY entry;


        sprintf(buf_key_table,"_%d_%d",depth, root);
        std::string key_table=obj_pos.m_key + buf_key_table;





        if (m_tp_score.find(key_table) == m_tp_score.end())
            entry={-MATE_UPPER, MATE_UPPER};
        else{
            entry=m_tp_score[key_table]; //.value();
            m_claves_score_reusadas++;
        }

        //m_tp_move[obj_pos.m_key]=std::nullopt;

        if (entry.lower >= gamma && (root==false || valid_move_in_table(obj_pos.m_key) ))
            return entry.lower;
        if (entry.upper < gamma)
            return entry.upper;


        // Run through the moves, shortcutting when possible
        int best = -MATE_UPPER;
        bool gamma_break=false;

        // First try not moving at all. We only do this if there is at least one major
        //piece left on the board, since otherwise zugzwangs are too dangerous.
        if (depth > 0 && root == false && obj_pos.has_major_pieces()){
            CPosition obj_null_move = obj_pos.nullmove();
            int score = -bound(obj_null_move, 1-gamma, depth-3, false);
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
                obj_pos.render_nnue_arrays(m_nue_pieces , m_nue_squares);
                score= nnue_evaluate(0,m_nue_pieces,m_nue_squares);
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
                MOVE killer = m_tp_move[obj_pos.m_key].value();
                if (depth > 0 || obj_pos.get_value_move(killer) >= QS_LIMIT){
                    CPosition pos_moved = obj_pos.make_move(killer);
                    int score = -bound(pos_moved, 1-gamma, depth-1, false);
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

        std::vector<MOVE> lst_moves;
        //lst_moves.reserve(40);
        if (gamma_break==false){
            auto start = std::chrono::high_resolution_clock::now();
            obj_pos.gen_moves(lst_moves);

            // Ordenar la lista de mayor a menor por el campo value
            std::stable_sort(lst_moves.begin(), lst_moves.end(), orderMovesByValue);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

            g_time1 += (double) duration.count();

             for (const MOVE& move : lst_moves) {
//            for move in sorted(pos.gen_moves(), key=pos.get_value_move, reverse=True):
                if (depth > 0 || move.value >= QS_LIMIT){
                    CPosition pos_moved = obj_pos.make_move(move);
                    int score= -bound(pos_moved, 1-gamma, depth-1, false);
                    best = std::max(best, score);
                    if (best >= gamma){
                        if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                        m_tp_move[obj_pos.m_key] = move;
                        break;
                    }
                }
             }
        }
/*
        # Stalemate checking is a bit tricky: Say we failed low, because
        # we can't (legally) move and so the (real) score is -infty.
        # At the next depth we are allowed to just return r, -infty <= r < gamma,
        # which is normally fine.
        # However, what if gamma = -10 and we don't have any legal moves?
        # Then the score is actaully a draw and we should fail high!
        # Thus, if best < gamma and best < 0 we need to double check what we are doing.
        # This doesn't prevent numbfish from making a move that results in stalemate,
        # but only if depth == 1, so that's probably fair enough.
        # (Btw, at depth 1 we can also mate without realizing.)
*/
        if (best < gamma && best < 0 && depth > 0){
            if (lst_moves.empty())
                obj_pos.gen_moves(lst_moves);
            bool all_dead=true;
            for (const MOVE& move : lst_moves){
                CPosition pos_moved = obj_pos.make_move(move);
                if ( pos_moved.is_dead()==false){
                    all_dead=false;
                    break;
                }
            }
            if (all_dead){
                CPosition pos_null = obj_pos.nullmove();
                bool in_check = pos_null.is_dead();
                if (in_check)
                    best = -MATE_UPPER;
                else best = 0;
            }
        }

        // Clear before setting, so we always have a value
        if (m_tp_score.size() > TABLE_SIZE) m_tp_score.clear();
        //Table part 2
        if (best >= gamma)
            m_tp_score[key_table] = {best, entry.upper};
        if (best < gamma)
            m_tp_score[key_table] = {entry.lower, best};


        return best;


}

void ordenarPorValue(MOVE* arr, int n) {
    for (int i = 1; i < n; ++i) {
        MOVE key = arr[i];
        int j = i - 1;

        // Mueve los elementos menores que key una posición hacia la derecha
        while (j >= 0 && arr[j].value < key.value) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
int NumbFishProbe::bound2(CPosition &obj_pos, int gamma, int depth,int idxbegin_moves,bool root)
{
        if (m_time_out  || m_state == STATE_STOP)
            return 0;
        m_nodes++;
        if (m_nodes%50000==0){
            m_time_out = is_time_out();
            if (m_time_out)
                return 0;
        }


        if (idxbegin_moves>MAX_MOVES_MEMORY){

                return 0;

        }
        depth = std::max(depth, 0);
        //POSI *pos = obj_pos.get_posi_struct();
        if (obj_pos.get_score() <= -MATE_LOWER)
            return -MATE_UPPER;

        if (obj_pos.m_key[0]=='\0'){

            obj_pos.make_key();

        }

        //if ( root == false && pos->board in self.history)
        //    return 0

        /* Look in the table if we have already searched this position before.
         We also need to be sure, that the stored search was over the same
         nodes as the current search. */
        TABLE_ENTRY entry;


        sprintf(buf_key_table,"_%d_%d",depth, root);
        std::string key_table=obj_pos.m_key + buf_key_table;





        if (m_tp_score.find(key_table) == m_tp_score.end())
            entry={-MATE_UPPER, MATE_UPPER};
        else{
            entry=m_tp_score[key_table]; //.value();
            m_claves_score_reusadas++;
        }

        //m_tp_move[obj_pos.m_key]=std::nullopt;

        if (entry.lower >= gamma && (root==false || valid_move_in_table(obj_pos.m_key) ))
            return entry.lower;
        if (entry.upper < gamma)
            return entry.upper;


        // Run through the moves, shortcutting when possible
        int best = -MATE_UPPER;
        bool gamma_break=false;

        // First try not moving at all. We only do this if there is at least one major
        //piece left on the board, since otherwise zugzwangs are too dangerous.
        if (depth > 0 && root == false && obj_pos.has_major_pieces()){
            CPosition obj_null_move = obj_pos.nullmove();
            int score = -bound2(obj_null_move, 1-gamma, depth-3, idxbegin_moves,false);
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
                obj_pos.render_nnue_arrays(m_nue_pieces , m_nue_squares);
                score= nnue_evaluate(0,m_nue_pieces,m_nue_squares);
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
                MOVE killer = m_tp_move[obj_pos.m_key].value();
                if (depth > 0 || obj_pos.get_value_move(killer) >= QS_LIMIT){
                    CPosition pos_moved = obj_pos.make_move(killer);
                    int score = -bound2(pos_moved, 1-gamma, depth-1,idxbegin_moves, false);
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
        //MOVE *arr_moves= new MOVE[100];
        //MOVE arr_moves[100];
        int nmoves=0;
        MOVE *p_moves = &arr_moves[idxbegin_moves];
        if (gamma_break==false){
            auto start = std::chrono::high_resolution_clock::now();
            nmoves=obj_pos.gen_arr_moves(p_moves);

            // Ordenar la lista de mayor a menor por el campo value
            if (nmoves){
                ordenarPorValue(p_moves,nmoves);
                idxbegin_moves+=nmoves;
            }
            //std::sort(lst_moves.begin(), lst_moves.end(), orderMovesByValue);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

            g_time1 += (double) duration.count();

             for (int i=0; i < nmoves ; i++) {

//            for move in sorted(pos.gen_moves(), key=pos.get_value_move, reverse=True):
                if (depth > 0 || p_moves[i].value >= QS_LIMIT){
                    CPosition pos_moved = obj_pos.make_move(p_moves[i]);
                    int score= -bound2(pos_moved, 1-gamma, depth-1,idxbegin_moves, false);
                    best = std::max(best, score);
                    if (best >= gamma){
                        if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                        m_tp_move[obj_pos.m_key] = p_moves[i];
                        break;
                    }
                }
             }
        }
/*
        # Stalemate checking is a bit tricky: Say we failed low, because
        # we can't (legally) move and so the (real) score is -infty.
        # At the next depth we are allowed to just return r, -infty <= r < gamma,
        # which is normally fine.
        # However, what if gamma = -10 and we don't have any legal moves?
        # Then the score is actaully a draw and we should fail high!
        # Thus, if best < gamma and best < 0 we need to double check what we are doing.
        # This doesn't prevent numbfish from making a move that results in stalemate,
        # but only if depth == 1, so that's probably fair enough.
        # (Btw, at depth 1 we can also mate without realizing.)
*/
        if (best < gamma && best < 0 && depth > 0){
            if (nmoves==0){
                nmoves =obj_pos.gen_arr_moves(p_moves);
                idxbegin_moves+=nmoves;
            }
            bool all_dead=true;
            for (int i=0; i < nmoves ; i++) {
                CPosition pos_moved = obj_pos.make_move(p_moves[i]);
                if ( pos_moved.is_dead()==false){
                    all_dead=false;
                    break;
                }
            }
            if (all_dead){
                CPosition pos_null = obj_pos.nullmove();
                bool in_check = pos_null.is_dead();
                if (in_check)
                    best = -MATE_UPPER;
                else best = 0;
            }
        }

        // Clear before setting, so we always have a value
        if (m_tp_score.size() > TABLE_SIZE) m_tp_score.clear();
        //Table part 2
        if (best >= gamma)
            m_tp_score[key_table] = {best, entry.upper};
        if (best < gamma)
            m_tp_score[key_table] = {entry.lower, best};

        //delete []arr_moves;
        idxbegin_moves -= nmoves;
        return best;


}
int NumbFishProbe::bound3(CPosition &obj_pos, int gamma, int depth,bool root)
{
        if (m_time_out)
            return 0;
        m_nodes++;
        if (m_nodes%10000==0 || m_state == STATE_STOP){
            m_time_out = is_time_out();
            if (m_time_out)
                return 0;
        }

        depth = std::max(depth, 0);
        //POSI *pos = obj_pos.get_posi_struct();
        if (obj_pos.get_score() <= -MATE_LOWER)
            return -MATE_UPPER;

        if (obj_pos.m_key[0]=='\0'){

            obj_pos.make_key();

        }

        //if ( root == false && pos->board in self.history)
        //    return 0

        /* Look in the table if we have already searched this position before.
         We also need to be sure, that the stored search was over the same
         nodes as the current search. */
        TABLE_ENTRY entry;


        sprintf(buf_key_table,"_%d_%d",depth, root);
        std::string key_table=obj_pos.m_key + buf_key_table;





        if (m_tp_score.find(key_table) == m_tp_score.end())
            entry={-MATE_UPPER, MATE_UPPER};
        else{
            entry=m_tp_score[key_table]; //.value();
            m_claves_score_reusadas++;
        }

        //m_tp_move[obj_pos.m_key]=std::nullopt;

        if (entry.lower >= gamma && (root==false || valid_move_in_table(obj_pos.m_key) ))
            return entry.lower;
        if (entry.upper < gamma)
            return entry.upper;


        // Run through the moves, shortcutting when possible
        int best = -MATE_UPPER;
        bool gamma_break=false;

        // First try not moving at all. We only do this if there is at least one major
        //piece left on the board, since otherwise zugzwangs are too dangerous.
        if (depth > 0 && root == false && obj_pos.has_major_pieces()){
            CPosition obj_null_move = obj_pos.nullmove();
            int score = -bound3(obj_null_move, 1-gamma, depth-3, false);
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
                obj_pos.render_nnue_arrays(m_nue_pieces , m_nue_squares);
                score= nnue_evaluate(0,m_nue_pieces,m_nue_squares);
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
                MOVE killer = m_tp_move[obj_pos.m_key].value();
                if (depth > 0 || obj_pos.get_value_move(killer) >= QS_LIMIT){
                    CPosition pos_moved = obj_pos.make_move(killer);
                    int score = -bound3(pos_moved, 1-gamma, depth-1,false);
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
        //MOVE *arr_moves= new MOVE[100];
        //MOVE arr_moves[100];
        int nmoves=0;
        MOVE *arr_moves= new MOVE[120];
        if (gamma_break==false){
            auto start = std::chrono::high_resolution_clock::now();
            nmoves=obj_pos.gen_arr_moves(arr_moves);

            // Ordenar la lista de mayor a menor por el campo value
            if (nmoves){
                ordenarPorValue(arr_moves,nmoves);
            }
            //std::sort(lst_moves.begin(), lst_moves.end(), orderMovesByValue);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

            g_time1 += (double) duration.count();

             for (int i=0; i < nmoves ; i++) {

//            for move in sorted(pos.gen_moves(), key=pos.get_value_move, reverse=True):
                if (depth > 0 || arr_moves[i].value >= QS_LIMIT){
                    CPosition pos_moved = obj_pos.make_move(arr_moves[i]);
                    int score= -bound3(pos_moved, 1-gamma, depth-1,false);
                    best = std::max(best, score);
                    if (best >= gamma){
                        if (m_tp_move.size() > TABLE_SIZE) m_tp_move.clear();
                        m_tp_move[obj_pos.m_key] = arr_moves[i];
                        break;
                    }
                }
             }
        }
/*
        # Stalemate checking is a bit tricky: Say we failed low, because
        # we can't (legally) move and so the (real) score is -infty.
        # At the next depth we are allowed to just return r, -infty <= r < gamma,
        # which is normally fine.
        # However, what if gamma = -10 and we don't have any legal moves?
        # Then the score is actaully a draw and we should fail high!
        # Thus, if best < gamma and best < 0 we need to double check what we are doing.
        # This doesn't prevent numbfish from making a move that results in stalemate,
        # but only if depth == 1, so that's probably fair enough.
        # (Btw, at depth 1 we can also mate without realizing.)
*/
        if (best < gamma && best < 0 && depth > 0){
            if (nmoves==0){
                nmoves =obj_pos.gen_arr_moves(arr_moves);

            }
            bool all_dead=true;
            for (int i=0; i < nmoves ; i++) {
                CPosition pos_moved = obj_pos.make_move(arr_moves[i]);
                if ( pos_moved.is_dead()==false){
                    all_dead=false;
                    break;
                }
            }
            if (all_dead){
                CPosition pos_null = obj_pos.nullmove();
                bool in_check = pos_null.is_dead();
                if (in_check)
                    best = -MATE_UPPER;
                else best = 0;
            }
        }

        // Clear before setting, so we always have a value
        if (m_tp_score.size() > TABLE_SIZE) m_tp_score.clear();
        //Table part 2
        if (best >= gamma)
            m_tp_score[key_table] = {best, entry.upper};
        if (best < gamma)
            m_tp_score[key_table] = {entry.lower, best};

        //delete []arr_moves;
        delete []arr_moves;
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

        int lower=-MATE_UPPER;
        int upper = MATE_UPPER;
        int gamma=0,score=0;
        while (lower < upper - EVAL_ROUGHNESS){
            gamma = (lower+upper+1)/2;
            //score = bound(pos, gamma, depth);

            score = bound2(pos, gamma, depth,0, true);
            m_time_out = is_time_out();
            if (m_time_out || m_state == STATE_STOP)
                break;
            if (score >= gamma)
                lower = score;
            if (score < gamma)
                upper = score;



        }
        if (!m_time_out && m_state!= STATE_STOP){
            //bound(pos, lower, depth);
            bound2(pos, lower, depth,0,true);

            sprintf(buf_key_table,"_%d_%d",depth, true);
            std::string key_table=pos.m_key + buf_key_table;
            if (m_tp_score.find(key_table) != m_tp_score.end())
                m_score = m_tp_score[key_table].lower;
        }



}

void NumbFishProbe::get_move(const std::string &fen, double secs)
{

    m_tp_score.clear();
    m_tp_move.clear();
    m_score=0;
    m_nodes=0;
    m_result_move="";
    m_depth=0;

    if (m_use_book){
        std::string uci_book_move =m_pbook->get_random_book_move(fen);
        if (uci_book_move !=""){
            //std::cout << "\n\tbook move " << uci_book_move;
            m_result_move = uci_book_move;
            return;

        }else
            m_use_book=false;
    }

    m_state = STATE_SEARCHING;
    CPosition pos;
    pos.parseFEN(fen.c_str());
    //std::cout << "\nINITIAL SCORE " << pos.get_score();

    int depth;

    std::string uci_move;
    m_time_out=false;
    m_move_time=secs;
    m_time_start = clock();

    //std::cout << "\n\nSEARCHING MOVE";
    for (depth = 1;  depth<100; depth++){
        mtdf_gamma(pos,depth);
        if (m_time_out || m_state==STATE_STOP) break;
        MOVE m=m_tp_move[pos.m_key].value();
        uci_move = pos.move_render(m);

        //std::cout << "\ndepth " << depth << " " << uci_move << " score " << m_score << " nodes " << m_nodes;

        if (is_time_out()|| m_score >=MATE_UPPER || m_state==STATE_STOP)
            break;

    }
    m_depth=depth;


    /*std::cout << "\nNodos " << m_nodes;

    std::cout << "\ntabla movimientos " << m_tp_move.size();
    std::cout << "\ntabla scores " << m_tp_score.size();

    std::cout << "\nt1  " << g_time1/1e9;
    std::cout << "\nt2  " << g_time2/1e9;
*/
    m_result_move = uci_move;
    m_state = STATE_END_SEARCH;
    //std::cout << "\n\result move " << uci_move << " score " << m_score;
    //return m_result_move;

}
