#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <vector>
#include "Board/board.hpp"
#include "Board/move.hpp"

void generate_pawn_moves(const Board& board, std::vector<Move>& move_list);
void generate_knight_moves(const Board& board, std::vector<Move>& move_list);
void generate_king_moves(const Board& board, std::vector<Move>& move_list);
void generate_bishop_moves(const Board& board, std::vector<Move>& move_list);
void generate_rook_moves(const Board& board, std::vector<Move>& move_list);
void generate_queen_moves(const Board& board, std::vector<Move>& move_list);
void generate_castling_moves(const Board& board, std::vector<Move>& move_list);

void generate_all_moves(const Board& board, std::vector<Move>& move_list);

#endif