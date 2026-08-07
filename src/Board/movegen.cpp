#include "Board/movegen.hpp"
#include "Board/attacks.hpp"

void generate_pawn_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    U64 pawns = board.piece_bitboards[us][PAWN];

    while (pawns) {
        int source = __builtin_ctzll(pawns);
        pop_bit(pawns, source);

        int target = (us == WHITE) ? (source + 8) : (source - 8);

        if (target >= A1 && target <= H8 && !get_bit(board.occupancies[BOTH], target)) {
            bool is_promo = (us == WHITE) ? (target >= A8) : (target <= H1);

            if (is_promo) {
                move_list.push_back(encode_move(source, target, FLAG_PROMO_QUEEN));
                move_list.push_back(encode_move(source, target, FLAG_PROMO_ROOK));
                move_list.push_back(encode_move(source, target, FLAG_PROMO_BISHOP));
                move_list.push_back(encode_move(source, target, FLAG_PROMO_KNIGHT));
            } else {
                move_list.push_back(encode_move(source, target, FLAG_QUIET));

                bool is_start_rank = (us == WHITE) ? (source >= A2 && source <= H2) : (source >= A7 && source <= H7);
                int double_target = (us == WHITE) ? (source + 16) : (source - 16);

                if (is_start_rank && !get_bit(board.occupancies[BOTH], double_target)) {
                    move_list.push_back(encode_move(source, double_target, FLAG_DOUBLE_PUSH));
                }
            }
        }

        U64 attacks = pawn_attacks[us][source] & board.occupancies[them];

        while (attacks) {
            int cap_target = __builtin_ctzll(attacks);
            pop_bit(attacks, cap_target);

            bool is_promo = (us == WHITE) ? (cap_target >= A8) : (cap_target <= H1);

            if (is_promo) {
                move_list.push_back(encode_move(source, cap_target, FLAG_PROMO_CAP_QUEEN));
                move_list.push_back(encode_move(source, cap_target, FLAG_PROMO_CAP_ROOK));
                move_list.push_back(encode_move(source, cap_target, FLAG_PROMO_CAP_BISHOP));
                move_list.push_back(encode_move(source, cap_target, FLAG_PROMO_CAP_KNIGHT));
            } else {
                move_list.push_back(encode_move(source, cap_target, FLAG_CAPTURE));
            }
        }

        if (board.en_passant != SQ_NONE) {
            U64 ep_attack = pawn_attacks[us][source] & (1ULL << board.en_passant);
            if (ep_attack) {
                move_list.push_back(encode_move(source, board.en_passant, FLAG_EN_PASSANT));
            }
        }
    }
}

void generate_knight_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    U64 knights = board.piece_bitboards[us][KNIGHT];

    while (knights) {
        int source = __builtin_ctzll(knights);
        pop_bit(knights, source);

        U64 attacks = knight_attacks[source] & ~board.occupancies[us];

        while (attacks) {
            int target = __builtin_ctzll(attacks);
            pop_bit(attacks, target);

            int flags = get_bit(board.occupancies[them], target) ? FLAG_CAPTURE : FLAG_QUIET;
            move_list.push_back(encode_move(source, target, flags));
        }
    }
}

void generate_king_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    U64 king = board.piece_bitboards[us][KING];

    if (king) {
        int source = __builtin_ctzll(king);

        U64 attacks = king_attacks[source] & ~board.occupancies[us];

        while (attacks) {
            int target = __builtin_ctzll(attacks);
            pop_bit(attacks, target);

            int flags = get_bit(board.occupancies[them], target) ? FLAG_CAPTURE : FLAG_QUIET;
            move_list.push_back(encode_move(source, target, flags));
        }
    }
}

void generate_bishop_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    U64 bishops = board.piece_bitboards[us][BISHOP];

    while (bishops) {
        int source = __builtin_ctzll(bishops);
        pop_bit(bishops, source);

        U64 attacks = get_bishop_attacks(source, board.occupancies[BOTH]) & ~board.occupancies[us];

        while (attacks) {
            int target = __builtin_ctzll(attacks);
            pop_bit(attacks, target);

            int flags = get_bit(board.occupancies[them], target) ? FLAG_CAPTURE : FLAG_QUIET;
            move_list.push_back(encode_move(source, target, flags));
        }
    }
}

void generate_rook_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    U64 rooks = board.piece_bitboards[us][ROOK];

    while (rooks) {
        int source = __builtin_ctzll(rooks);
        pop_bit(rooks, source);

        U64 attacks = get_rook_attacks(source, board.occupancies[BOTH]) & ~board.occupancies[us];

        while (attacks) {
            int target = __builtin_ctzll(attacks);
            pop_bit(attacks, target);

            int flags = get_bit(board.occupancies[them], target) ? FLAG_CAPTURE : FLAG_QUIET;
            move_list.push_back(encode_move(source, target, flags));
        }
    }
}

void generate_queen_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    U64 queens = board.piece_bitboards[us][QUEEN];

    while (queens) {
        int source = __builtin_ctzll(queens);
        pop_bit(queens, source);

        U64 attacks = get_queen_attacks(source, board.occupancies[BOTH]) & ~board.occupancies[us];

        while (attacks) {
            int target = __builtin_ctzll(attacks);
            pop_bit(attacks, target);

            int flags = get_bit(board.occupancies[them], target) ? FLAG_CAPTURE : FLAG_QUIET;
            move_list.push_back(encode_move(source, target, flags));
        }
    }
}

void generate_castling_moves(const Board& board, std::vector<Move>& move_list)
{
    Color us = board.side_to_move;
    Color them = (us == WHITE) ? BLACK : WHITE;

    if (us == WHITE) {
        if (board.castling_rights & WK) {
            if (!get_bit(board.occupancies[BOTH], F1) && !get_bit(board.occupancies[BOTH], G1)) {
                if (!board.is_square_attacked(E1, them) && 
                    !board.is_square_attacked(F1, them) && 
                    !board.is_square_attacked(G1, them)) {
                    move_list.push_back(encode_move(E1, G1, FLAG_KING_CASTLE));
                }
            }
        }
        if (board.castling_rights & WQ) {
            if (!get_bit(board.occupancies[BOTH], D1) && 
                !get_bit(board.occupancies[BOTH], C1) && 
                !get_bit(board.occupancies[BOTH], B1)) {
                if (!board.is_square_attacked(E1, them) && 
                    !board.is_square_attacked(D1, them) && 
                    !board.is_square_attacked(C1, them)) {
                    move_list.push_back(encode_move(E1, C1, FLAG_QUEEN_CASTLE));
                }
            }
        }
    } else {
        if (board.castling_rights & BK) {
            if (!get_bit(board.occupancies[BOTH], F8) && !get_bit(board.occupancies[BOTH], G8)) {
                if (!board.is_square_attacked(E8, them) && 
                    !board.is_square_attacked(F8, them) && 
                    !board.is_square_attacked(G8, them)) {
                    move_list.push_back(encode_move(E8, G8, FLAG_KING_CASTLE));
                }
            }
        }
        if (board.castling_rights & BQ) {
            if (!get_bit(board.occupancies[BOTH], D8) && 
                !get_bit(board.occupancies[BOTH], C8) && 
                !get_bit(board.occupancies[BOTH], B8)) {
                if (!board.is_square_attacked(E8, them) && 
                    !board.is_square_attacked(D8, them) && 
                    !board.is_square_attacked(C8, them)) {
                    move_list.push_back(encode_move(E8, C8, FLAG_QUEEN_CASTLE));
                }
            }
        }
    }
}

void generate_all_moves(const Board& board, std::vector<Move>& move_list)
{
    move_list.clear();
    generate_pawn_moves(board, move_list);
    generate_knight_moves(board, move_list);
    generate_bishop_moves(board, move_list);
    generate_rook_moves(board, move_list);
    generate_queen_moves(board, move_list);
    generate_king_moves(board, move_list);
    generate_castling_moves(board, move_list);
}