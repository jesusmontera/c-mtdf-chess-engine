#ifndef SYZYGYONLINE_H
#define SYZYGYONLINE_H
#include <chess.hpp>

class SyzygyOnline
{
    public:
        SyzygyOnline();
        virtual ~SyzygyOnline();
        bool query_lichess_syzygy(const chess::Board &board, chess::Move &move);
    protected:

    private:
        char * Query(char *hostname, char *message) const;
};

#endif // SYZYGYONLINE_H
