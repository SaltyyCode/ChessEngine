#ifndef SEARCH_HPP
#define SEARCH_HPP

#include "Board/board.hpp"
#include "Board/move.hpp"

int quiescence(Board& board, int alpha, int beta);
Move search_best_move(Board& board, int depth);

#endif