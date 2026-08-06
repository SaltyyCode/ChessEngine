#include "Board/board.hpp"


Piece Board::get_piece_at(int sq, Color color) const
{
    for (int p = PAWN; p <= KING; ++p) {
        if (get_bit(piece_bitboards[color][p], sq)) {
            return static_cast<Piece>(p);
        }
    }
    return PAWN;
}


void Board::remove_captured_piece(int target, int flags, Color them)
{
    if (flags == FLAG_EN_PASSANT) {
        int ep_pawn_sq = (them == BLACK) ? (target - 8) : (target + 8);
        pop_bit(piece_bitboards[them][PAWN], ep_pawn_sq);
    } else {
        Piece captured = get_piece_at(target, them);
        pop_bit(piece_bitboards[them][captured], target);
    }
}


void Board::place_moved_piece(int target, Piece moved_piece, int flags, Color us)
{
    if (flags >= FLAG_PROMO_KNIGHT) {
        Piece promo_piece = QUEEN;
        if (flags == FLAG_PROMO_KNIGHT || flags == FLAG_PROMO_CAP_KNIGHT) promo_piece = KNIGHT;
        else if (flags == FLAG_PROMO_BISHOP || flags == FLAG_PROMO_CAP_BISHOP) promo_piece = BISHOP;
        else if (flags == FLAG_PROMO_ROOK   || flags == FLAG_PROMO_CAP_ROOK)   promo_piece = ROOK;

        set_bit(piece_bitboards[us][promo_piece], target);
    } else {
        set_bit(piece_bitboards[us][moved_piece], target);
    }
}


void Board::move_castling_rook(int flags, Color us)
{
    if (flags == FLAG_KING_CASTLE) {
        if (us == WHITE) { pop_bit(piece_bitboards[WHITE][ROOK], H1); set_bit(piece_bitboards[WHITE][ROOK], F1); }
        else             { pop_bit(piece_bitboards[BLACK][ROOK], H8); set_bit(piece_bitboards[BLACK][ROOK], F8); }
    } else if (flags == FLAG_QUEEN_CASTLE) {
        if (us == WHITE) { pop_bit(piece_bitboards[WHITE][ROOK], A1); set_bit(piece_bitboards[WHITE][ROOK], D1); }
        else             { pop_bit(piece_bitboards[BLACK][ROOK], A8); set_bit(piece_bitboards[BLACK][ROOK], D8); }
    }
}

void Board::update_castling_rights(int source, int target, Piece moved_piece, Color us)
{
    if (moved_piece == KING) {
        castling_rights &= (us == WHITE) ? ~(WK | WQ) : ~(BK | BQ);
    }
    if (source == A1 || target == A1) castling_rights &= ~WQ;
    if (source == H1 || target == H1) castling_rights &= ~WK;
    if (source == A8 || target == A8) castling_rights &= ~BQ;
    if (source == H8 || target == H8) castling_rights &= ~BK;
}


void Board::make_move(Move move)
{
    int source = get_move_source(move);
    int target = get_move_target(move);
    int flags  = get_move_flags(move);

    Color us = side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    Piece moved_piece = get_piece_at(source, us);
    pop_bit(piece_bitboards[us][moved_piece], source);

    if (is_capture(move)) {
        remove_captured_piece(target, flags, them);
    }

    place_moved_piece(target, moved_piece, flags, us);
    move_castling_rook(flags, us);

    en_passant = (flags == FLAG_DOUBLE_PUSH)
        ? static_cast<Square>((us == WHITE) ? (target - 8) : (target + 8))
        : SQ_NONE;

    update_castling_rights(source, target, moved_piece, us);
    side_to_move = them;
    update_occupancies();
}