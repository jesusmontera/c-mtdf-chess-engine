#ifndef CNNUEEVALUATOR_H
#define CNNUEEVALUATOR_H


class CnnueEvaluator
{
    public:
        CnnueEvaluator();
        virtual ~CnnueEvaluator();
        int get_nnue_board_value(const char *board);
    protected:

    private:
        int *m_pieces;
        int *m_squares;

};

#endif // CNNUEEVALUATOR_H
