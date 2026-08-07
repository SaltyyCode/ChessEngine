#include "Search/search.hpp"
#include "Board/movegen.hpp"
#include "Eval/eval.hpp"
#include <vector>

constexpr int INF = 1000000;

int quiescence(Board& board, int alpha, int beta)
{

    int stand_pat = evaluate(board);

    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    std::vector<Move> move_list;
    generate_all_moves(board, move_list);

    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    for (Move move : move_list) {

        int flags = get_move_flags(move);
        if (flags != FLAG_CAPTURE) continue;

        board.make_move(move);

        int king_sq = __builtin_ctzll(board.piece_bitboards[us][KING]);

        if (!board.is_square_attacked(king_sq, them)) {
            int score = -quiescence(board, -beta, -alpha);
            board.unmake_move(move);

            if (score >= beta) return beta;
            if (score > alpha) alpha = score;
        } else {
            board.unmake_move(move);
        }
    }

    return alpha;
}

int alphabeta(Board& board, int alpha, int beta, int depth)
{
    
    if (depth == 0) {
        return quiescence(board, alpha, beta);
    }

    std::vector<Move> move_list;
    generate_all_moves(board, move_list);

    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    int legal_moves = 0;
    int best_score = -INF;

    for (Move move : move_list) {
        board.make_move(move);

        int king_sq = __builtin_ctzll(board.piece_bitboards[us][KING]);

        if (!board.is_square_attacked(king_sq, them)) {
            legal_moves++;
            int score = -alphabeta(board, -beta, -alpha, depth - 1);
            board.unmake_move(move);

            if (score > best_score) {
                best_score = score;
            }
            if (score > alpha) {
                alpha = score;
            }
            if (alpha >= beta) {
                return alpha;
            }
        } else {
            board.unmake_move(move);
        }
    }

    if (legal_moves == 0) {
        int king_sq = __builtin_ctzll(board.piece_bitboards[us][KING]);
        if (board.is_square_attacked(king_sq, them)) {
            return -INF + (100 - depth);
        } else {
            return 0;
        }
    }

    return best_score;
}

Move search_best_move(Board& board, int depth)
{
    std::vector<Move> move_list;
    generate_all_moves(board, move_list);

    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    Move best_move = 0;
    int best_score = -INF;
    int alpha = -INF;
    int beta = INF;

    for (Move move : move_list) {
        board.make_move(move);

        int king_sq = __builtin_ctzll(board.piece_bitboards[us][KING]);

        if (!board.is_square_attacked(king_sq, them)) {
            int score = -alphabeta(board, -beta, -alpha, depth - 1);
            board.unmake_move(move);

            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
            if (score > alpha) {
                alpha = score;
            }
        } else {
            board.unmake_move(move);
        }
    }

    return best_move;
}