#ifndef EVAL_HPP
#define EVAL_HPP

#include "Board/board.hpp"

constexpr int PAWN_VAL = 100;
constexpr int KNIGHT_VAL = 320;
constexpr int BISHOP_VAL = 330;
constexpr int ROOK_VAL = 500;
constexpr int QUEEN_VAL = 900;
constexpr int KING_VAL = 20000;

int evaluate(const Board& board);

#endif