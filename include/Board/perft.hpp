#ifndef PERFT_HPP
#define PERFT_HPP

#include "Board/board.hpp"

U64 perft(int depth, Board& board);
void perft_divide(int depth, Board& board);

#endif