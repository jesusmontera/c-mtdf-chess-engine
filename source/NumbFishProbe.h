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
struct MOVE_SCORE_DEPTH
{
    Move move;
    int score;
    int depth;
};
//#define NUM_LETTERS_BOARD 120
struct KEY_TABLE_SCORE {
  std::uint64_t key;
  int depth;
  bool is_root;

  /* required by unordered_map */
    bool operator ==(const KEY_TABLE_SCORE& other) const {
        return (this->key == other.key) &&
            (this->depth == other.depth) &&
            (this->is_root == other.is_root);
    }
};
/* required to hash a KEY_TABLE_SCORE */
template<> struct std::hash<KEY_TABLE_SCORE> {
    std::size_t operator()(const KEY_TABLE_SCORE& ks) const {
        return ((hash<uint64_t>()(ks.key)
               ^ (hash<int>()(ks.depth) << 1)) >> 1)
               ^ (hash<bool>()(ks.is_root) << 2);
        //return ks.key ^ std::hash<int>()(ks.depth) ^
        //            std::hash<bool>()(ks.is_root);
    }
};

bool orderMovesByScore(const Move& a, const Move& b);
class NumbFishProbe
{
    public:
        int m_state;
        int m_claves_score_reusadas;
        int get_score(){return m_score;};
        int get_depth(){return m_depth;};
        int get_nodes(){return m_nodes;};
        std::string get_result_move(){return m_result_move;};

        //void get_move(const std::string &fen, double secs);
        void get_move(const Board &board, double secs,bool bprintinfo=false);
        NumbFishProbe(bool use_nnue, bool use_book);
        virtual ~NumbFishProbe();


    protected:

    private:
        bool m_time_out;
        clock_t m_time_start;
        double m_move_time;
        int m_depth;
        bool is_time_out();
        std::string m_result_move;
        bool m_use_book;
        int * m_nue_pieces;
        int *m_nue_squares;
        bool m_use_nnue;
        int m_nodes;
        int m_score;
        void print_scores() const;
        std::unordered_map<std::uint64_t, TABLE_ENTRY> m_tp_score[2][101];
        //std::unordered_map<KEY_TABLE_SCORE, TABLE_ENTRY> m_tp_score;
        //std::unordered_map<std::string, TABLE_ENTRY> m_tp_score;
        std::unordered_map<std::uint64_t,  std::optional<Move>> m_tp_move;
        bool valid_move_in_table(std::uint64_t &key);
        void mtdf_gamma(CPosition &pos, int depth);
        int bound(CPosition &pos, int gamma, int depth,bool root=true);
        void avoid_repetitions(const Board &board, std::vector<MOVE_SCORE_DEPTH> &msd );
        bool move_lichess_syzygy(const Board &board);
        bool move_opening_book(const Board &board);
        void clear_search_variables();
        //bool set_gamma_move(const CPosition &obj_pos, int best,int gamma,const Move &move);
        //bool set_gamma_move_null(const CPosition &obj_pos, int best,int gamma);
        Cbook *m_pbook;
};

#endif // NUMBFISHPROBE_H
