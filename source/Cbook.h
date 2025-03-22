#ifndef CBOOK_H
#define CBOOK_H

// Copyright (c) 2024 Shreyas Deo
#include "reader.hpp"



class Cbook
{
    public:
        Cbook(const char *book_file);
        virtual ~Cbook();
        std::string get_random_book_move(const std::string &fen);

    protected:

    private:
        Reader::Book m_book;

};

#endif // CBOOK_H
