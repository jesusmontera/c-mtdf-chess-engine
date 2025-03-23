#ifndef NUMBFISHPROBE_H
#define NUMBFISHPROBE_H
#include <bits/stdc++.h>
#include "CPosition.h"
#include <optional>
#include "Cbook.h"

#define STATE_FREE 0
#define STATE_END_SEARCH 2
#define STATE_SEARCHING 1
#define STATE_STOP 4

struct TABLE_ENTRY
{
    int lower;
    int upper;
};

class NumbFishProbe
{
    public:
        int m_claves_score_reusadas;
        void get_move(const std::string &fen, double secs);
        NumbFishProbe(bool use_nnue, bool use_book);
        virtual ~NumbFishProbe();
        int get_score(){return m_score;};
        int get_depth(){return m_depth;};
        int get_nodes(){return m_nodes;};
        std::string get_result_move(){return m_result_move;};
        int m_state;

    protected:

    private:
        bool m_time_out;
        bool is_time_out();
        clock_t m_time_start;
        double m_move_time;
        std::string m_result_move;
        bool m_use_book;
        int * m_nue_pieces;
        int *m_nue_squares;
        bool m_use_nnue;
        MOVE *arr_moves;
        int m_nodes;
        int m_score;
        int m_depth;
        std::unordered_map<std::string, TABLE_ENTRY> m_tp_score;
        std::unordered_map<std::string,  std::optional<MOVE>> m_tp_move;
        bool valid_move_in_table(std::string &key);
        void mtdf_gamma(CPosition &pos, int depth);
        int bound(CPosition &pos, int gamma, int depth,bool root=true);
        int bound2(CPosition &obj_pos, int gamma, int depth,int idxbegin_moves,bool root=true);
        int bound3(CPosition &obj_pos, int gamma, int depth,bool root=true);
        Cbook *m_pbook;
};

#endif // NUMBFISHPROBE_H
