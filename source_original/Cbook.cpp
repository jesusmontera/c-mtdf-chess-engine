#include "Cbook.h"
// Copyright (c) 2023 disservin
#include "chess.hpp" // to get the zobrish key from fen
using namespace chess;
Cbook::Cbook(const char *book_file)
{
    //const char book_file[]="/media/luis/48A0CF8FA0CF8244/luis/codigo phyton lubuntu/ajedrez/numbfish/Perfect2021.bin";
    m_book.Load(book_file);

}

Cbook::~Cbook()
{
     m_book.Clear();
}
std::string Cbook::get_random_book_move(const std::string &fen)
{

    Board board = Board(fen);

    std::uint64_t hash_key= board.hash() ;
    Reader::BookMoves book_moves = m_book.GetBookMoves(hash_key);
    std::string rnd_uci_move="";
    if (book_moves.size() > 0){
        rnd_uci_move= Reader::ConvertBookMoveToUci(Reader::RandomBookMove(book_moves));
       //Move movi = uciToMove(m_board, rnd_uci_move);
        //m_board.makeMove(movi);
    }
    return rnd_uci_move;


}
