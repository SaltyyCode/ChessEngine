#include "Eval/eval.hpp"

const int pawn_pst[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
   50, 50, 50, 50, 50, 50, 50, 50,
   10, 10, 20, 30, 30, 20, 10, 10,
    5,  5, 10, 25, 25, 10,  5,  5,
    0,  0,  0, 20, 20,  0,  0,  0,
    5, -5,-10,  0,  0,-10, -5,  5,
    5, 10, 10,-20,-20, 10, 10,  5,
    0,  0,  0,  0,  0,  0,  0,  0
};

const int knight_pst[64] = {
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50
};

const int bishop_pst[64] = {
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -20,-10,-10,-10,-10,-10,-10,-20
};

const int rook_pst[64] = {
    0,  0,  0,  0,  0,  0,  0,  0,
    5, 10, 10, 10, 10, 10, 10,  5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
   -5,  0,  0,  0,  0,  0,  0, -5,
    0,  0,  0,  5,  5,  0,  0,  0
};

const int queen_pst[64] = {
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5,  5,  5,  5,  0,-10,
   -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
  -10,  5,  5,  5,  5,  5,  0,-10,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

const int king_pst[64] = {
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -30,-40,-40,-50,-50,-40,-40,-30,
  -20,-30,-30,-40,-40,-30,-30,-20,
  -10,-20,-20,-20,-20,-20,-20,-10,
   20, 20,  0,  0,  0,  0, 20, 20,
   20, 30, 10,  0,  0, 10, 30, 20
};

int evaluate(const Board& board)
{
    int score_white = 0;
    int score_black = 0;

    U64 wp = board.piece_bitboards[WHITE][PAWN];
    while (wp) {
        int sq = __builtin_ctzll(wp);
        pop_bit(wp, sq);
        score_white += PAWN_VAL + pawn_pst[sq];
    }
    U64 bp = board.piece_bitboards[BLACK][PAWN];
    while (bp) {
        int sq = __builtin_ctzll(bp);
        pop_bit(bp, sq);
        score_black += PAWN_VAL + pawn_pst[sq ^ 56];
    }

    U64 wn = board.piece_bitboards[WHITE][KNIGHT];
    while (wn) {
        int sq = __builtin_ctzll(wn);
        pop_bit(wn, sq);
        score_white += KNIGHT_VAL + knight_pst[sq];
    }
    U64 bn = board.piece_bitboards[BLACK][KNIGHT];
    while (bn) {
        int sq = __builtin_ctzll(bn);
        pop_bit(bn, sq);
        score_black += KNIGHT_VAL + knight_pst[sq ^ 56];
    }

    U64 wb = board.piece_bitboards[WHITE][BISHOP];
    while (wb) {
        int sq = __builtin_ctzll(wb);
        pop_bit(wb, sq);
        score_white += BISHOP_VAL + bishop_pst[sq];
    }
    U64 bb = board.piece_bitboards[BLACK][BISHOP];
    while (bb) {
        int sq = __builtin_ctzll(bb);
        pop_bit(bb, sq);
        score_black += BISHOP_VAL + bishop_pst[sq ^ 56];
    }

    U64 wr = board.piece_bitboards[WHITE][ROOK];
    while (wr) {
        int sq = __builtin_ctzll(wr);
        pop_bit(wr, sq);
        score_white += ROOK_VAL + rook_pst[sq];
    }
    U64 br = board.piece_bitboards[BLACK][ROOK];
    while (br) {
        int sq = __builtin_ctzll(br);
        pop_bit(br, sq);
        score_black += ROOK_VAL + rook_pst[sq ^ 56];
    }

    U64 wq = board.piece_bitboards[WHITE][QUEEN];
    while (wq) {
        int sq = __builtin_ctzll(wq);
        pop_bit(wq, sq);
        score_white += QUEEN_VAL + queen_pst[sq];
    }
    U64 bq = board.piece_bitboards[BLACK][QUEEN];
    while (bq) {
        int sq = __builtin_ctzll(bq);
        pop_bit(bq, sq);
        score_black += QUEEN_VAL + queen_pst[sq ^ 56];
    }

    U64 wk = board.piece_bitboards[WHITE][KING];
    if (wk) {
        int sq = __builtin_ctzll(wk);
        score_white += KING_VAL + king_pst[sq];
    }
    U64 bk = board.piece_bitboards[BLACK][KING];
    if (bk) {
        int sq = __builtin_ctzll(bk);
        score_black += KING_VAL + king_pst[sq ^ 56];
    }

    if (get_bit(board.piece_bitboards[WHITE][KING], G1) || get_bit(board.piece_bitboards[WHITE][KING], C1)) {
        score_white += 60;
    } else if (board.castling_rights & (WK | WQ)) {
        score_white += 20;
    }

    if (get_bit(board.piece_bitboards[BLACK][KING], G8) || get_bit(board.piece_bitboards[BLACK][KING], C8)) {
        score_black += 60;
    } else if (board.castling_rights & (BK | BQ)) {
        score_black += 20;
    }

    int evaluation = score_white - score_black;
    return (board.side_to_move == WHITE) ? evaluation : -evaluation;
}