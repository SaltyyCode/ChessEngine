#include "Board/perft.hpp"
#include "Board/movegen.hpp"
#include "Board/attacks.hpp"
#include <iostream>
#include <chrono>

U64 perft(int depth, Board& board)
{
    if (depth == 0) return 1ULL;

    U64 nodes = 0ULL;
    std::vector<Move> move_list;
    generate_all_moves(board, move_list);

    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    for (Move move : move_list) {
        board.make_move(move);

        int king_sq = __builtin_ctzll(board.piece_bitboards[us][KING]);

        if (!board.is_square_attacked(king_sq, them)) {
            nodes += perft(depth - 1, board);
        }

        board.unmake_move(move);
    }

    return nodes;
}

void perft_divide(int depth, Board& board)
{
    if (depth == 0) return;

    U64 total_nodes = 0ULL;
    std::vector<Move> move_list;
    generate_all_moves(board, move_list);

    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::cout << "\n=== PERFT DIVIDE (Profondeur " << depth << ") ===\n";

    for (Move move : move_list) {
        board.make_move(move);

        int king_sq = __builtin_ctzll(board.piece_bitboards[us][KING]);

        if (!board.is_square_attacked(king_sq, them)) {
            U64 nodes = perft(depth - 1, board);
            total_nodes += nodes;

            int src = get_move_source(move);
            int tgt = get_move_target(move);

            char src_file = 'a' + (src % 8);
            char src_rank = '1' + (src / 8);
            char tgt_file = 'a' + (tgt % 8);
            char tgt_rank = '1' + (tgt / 8);

            std::cout << src_file << src_rank << tgt_file << tgt_rank << ": " << nodes << "\n";
        }

        board.unmake_move(move);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    std::cout << "\nNoeuds totaux : " << total_nodes << "\n";
    std::cout << "Temps : " << duration << " ms\n\n";
}