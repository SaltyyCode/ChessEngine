#include "Board/attacks.hpp"
#include "Board/board.hpp"
#include "Board/magics.hpp"

U64 pawn_attacks[2][64];
U64 knight_attacks[64];
U64 king_attacks[64];
U64 bishop_masks[64];
U64 rook_masks[64];
U64 bishop_attacks[64][512];
U64 rook_attacks[64][4096];

U64 mask_pawn_attacks(int side, int sq)
{
    U64 attacks = 0ULL;
    U64 bitboard = (1ULL << sq);

    if (side == 0) {
        if ((bitboard << 7) & NOT_H_FILE) attacks |= (bitboard << 7); 
        if ((bitboard << 9) & NOT_A_FILE) attacks |= (bitboard << 9);
    } else {
        if ((bitboard >> 7) & NOT_A_FILE) attacks |= (bitboard >> 7); 
        if ((bitboard >> 9) & NOT_H_FILE) attacks |= (bitboard >> 9);
    }

    return attacks;
}

U64 mask_knight_attacks(int sq)
{
    U64 attacks = 0ULL;
    U64 b = (1ULL << sq);


    if ((b << 17) & NOT_A_FILE) attacks |= (b << 17);
    if ((b << 15) & NOT_H_FILE) attacks |= (b << 15);
    if ((b << 10) & NOT_B_FILE) attacks |= (b << 10);
    if ((b <<  6) & NOT_G_FILE) attacks |= (b <<  6);

    if ((b >> 17) & NOT_H_FILE) attacks |= (b >> 17);
    if ((b >> 15) & NOT_A_FILE) attacks |= (b >> 15);
    if ((b >> 10) & NOT_G_FILE) attacks |= (b >> 10);
    if ((b >>  6) & NOT_B_FILE) attacks |= (b >>  6);

    return attacks;
}

U64 mask_king_attacks(int sq)
{
    U64 attacks = 0ULL;
    U64 b = (1ULL << sq);

    if (b << 8)                 attacks |= (b << 8);  // Haut
    if (b >> 8)                 attacks |= (b >> 8);  // Bas
    if ((b << 1) & NOT_A_FILE)  attacks |= (b << 1);  // Droite
    if ((b >> 1) & NOT_H_FILE)  attacks |= (b >> 1);  // Gauche

    if ((b << 9) & NOT_A_FILE)  attacks |= (b << 9);  // Haut-droite
    if ((b << 7) & NOT_H_FILE)  attacks |= (b << 7);  // Haut-gauche
    if ((b >> 7) & NOT_A_FILE)  attacks |= (b >> 7);  // Bas-droite
    if ((b >> 9) & NOT_H_FILE)  attacks |= (b >> 9);  // Bas-gauche

    return attacks;
}

void init_leaper_attacks()
{
    for (int sq = 0; sq < 64; ++sq) {
        pawn_attacks[0][sq] = mask_pawn_attacks(0, sq);
        pawn_attacks[1][sq] = mask_pawn_attacks(1, sq);
        knight_attacks[sq]  = mask_knight_attacks(sq);
        king_attacks[sq]    = mask_king_attacks(sq);
    }
}

U64 mask_bishop_attacks(int sq)
{
    U64 attacks = 0ULL;
    int r = sq / 8, f = sq % 8;

    for (int r_idx = r + 1, f_idx = f + 1; r_idx <= 6 && f_idx <= 6; ++r_idx, ++f_idx) attacks |= (1ULL << (r_idx * 8 + f_idx));
    for (int r_idx = r + 1, f_idx = f - 1; r_idx <= 6 && f_idx >= 1; ++r_idx, --f_idx) attacks |= (1ULL << (r_idx * 8 + f_idx));
    for (int r_idx = r - 1, f_idx = f + 1; r_idx >= 1 && f_idx <= 6; --r_idx, ++f_idx) attacks |= (1ULL << (r_idx * 8 + f_idx));
    for (int r_idx = r - 1, f_idx = f - 1; r_idx >= 1 && f_idx >= 1; --r_idx, --f_idx) attacks |= (1ULL << (r_idx * 8 + f_idx));

    return attacks;
}

U64 mask_rook_attacks(int sq)
{
    U64 attacks = 0ULL;
    int r = sq / 8, f = sq % 8;

    for (int r_idx = r + 1; r_idx <= 6; ++r_idx) attacks |= (1ULL << (r_idx * 8 + f));
    for (int r_idx = r - 1; r_idx >= 1; --r_idx) attacks |= (1ULL << (r_idx * 8 + f));
    for (int f_idx = f + 1; f_idx <= 6; ++f_idx) attacks |= (1ULL << (r * 8 + f_idx));
    for (int f_idx = f - 1; f_idx >= 1; --f_idx) attacks |= (1ULL << (r * 8 + f_idx));

    return attacks;
}

U64 bishop_attacks_on_the_fly(int sq, U64 block)
{
    U64 attacks = 0ULL;
    int r = sq / 8, f = sq % 8;

    for (int r_idx = r + 1, f_idx = f + 1; r_idx <= 7 && f_idx <= 7; ++r_idx, ++f_idx) {
        attacks |= (1ULL << (r_idx * 8 + f_idx));
        if ((1ULL << (r_idx * 8 + f_idx)) & block) break;
    }
    for (int r_idx = r + 1, f_idx = f - 1; r_idx <= 7 && f_idx >= 0; ++r_idx, --f_idx) {
        attacks |= (1ULL << (r_idx * 8 + f_idx));
        if ((1ULL << (r_idx * 8 + f_idx)) & block) break;
    }
    for (int r_idx = r - 1, f_idx = f + 1; r_idx >= 0 && f_idx <= 7; --r_idx, ++f_idx) {
        attacks |= (1ULL << (r_idx * 8 + f_idx));
        if ((1ULL << (r_idx * 8 + f_idx)) & block) break;
    }
    for (int r_idx = r - 1, f_idx = f - 1; r_idx >= 0 && f_idx >= 0; --r_idx, --f_idx) {
        attacks |= (1ULL << (r_idx * 8 + f_idx));
        if ((1ULL << (r_idx * 8 + f_idx)) & block) break;
    }

    return attacks;
}

U64 rook_attacks_on_the_fly(int sq, U64 block)
{
    U64 attacks = 0ULL;
    int r = sq / 8, f = sq % 8;

    for (int r_idx = r + 1; r_idx <= 7; ++r_idx) {
        attacks |= (1ULL << (r_idx * 8 + f));
        if ((1ULL << (r_idx * 8 + f)) & block) break;
    }
    for (int r_idx = r - 1; r_idx >= 0; --r_idx) {
        attacks |= (1ULL << (r_idx * 8 + f));
        if ((1ULL << (r_idx * 8 + f)) & block) break;
    }
    for (int f_idx = f + 1; f_idx <= 7; ++f_idx) {
        attacks |= (1ULL << (r * 8 + f_idx));
        if ((1ULL << (r * 8 + f_idx)) & block) break;
    }
    for (int f_idx = f - 1; f_idx >= 0; --f_idx) {
        attacks |= (1ULL << (r * 8 + f_idx));
        if ((1ULL << (r * 8 + f_idx)) & block) break;
    }

    return attacks;
}

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask)
{
    U64 occupancy = 0ULL;

    for (int count = 0; count < bits_in_mask; count++) {
        int sq = __builtin_ctzll(attack_mask);
        pop_bit(attack_mask, sq);

        if (index & (1 << count)) {
            occupancy |= (1ULL << sq);
        }
    }

    return occupancy;
}

void init_sliders_attacks(bool is_bishop)
{
    for (int sq = 0; sq < 64; ++sq) {
        bishop_masks[sq] = mask_bishop_attacks(sq);
        rook_masks[sq]   = mask_rook_attacks(sq);

        U64 mask = is_bishop ? bishop_masks[sq] : rook_masks[sq];
        int bit_count = __builtin_popcountll(mask);
        int occupancy_indices = (1 << bit_count);

        for (int index = 0; index < occupancy_indices; ++index) {
            if (is_bishop) {
                U64 occ = set_occupancy(index, bit_count, mask);
                int magic_index = (occ * bishop_magics[sq]) >> (64 - bishop_relevant_bits[sq]);
                bishop_attacks[sq][magic_index] = bishop_attacks_on_the_fly(sq, occ);
            } else {
                U64 occ = set_occupancy(index, bit_count, mask);
                int magic_index = (occ * rook_magics[sq]) >> (64 - rook_relevant_bits[sq]);
                rook_attacks[sq][magic_index] = rook_attacks_on_the_fly(sq, occ);
            }
        }
    }
}

void init_all_attacks()
{
    init_leaper_attacks();
    init_sliders_attacks(true);
    init_sliders_attacks(false);
}


U64 get_bishop_attacks(int sq, U64 occupancy)
{
    occupancy &= bishop_masks[sq];
    occupancy *= bishop_magics[sq];
    occupancy >>= (64 - bishop_relevant_bits[sq]);
    return bishop_attacks[sq][occupancy];
}

U64 get_rook_attacks(int sq, U64 occupancy)
{
    occupancy &= rook_masks[sq];
    occupancy *= rook_magics[sq];
    occupancy >>= (64 - rook_relevant_bits[sq]);
    return rook_attacks[sq][occupancy];
}

U64 get_queen_attacks(int sq, U64 occupancy)
{
    return get_bishop_attacks(sq, occupancy) | get_rook_attacks(sq, occupancy);
}

bool Board::is_square_attacked(int sq, Color side) const
{

    Color us = (side == WHITE) ? BLACK : WHITE;
    if (pawn_attacks[us][sq] & piece_bitboards[side][PAWN]) return true;

    if (knight_attacks[sq] & piece_bitboards[side][KNIGHT]) return true;

    if (king_attacks[sq] & piece_bitboards[side][KING]) return true;

    U64 bishop_eq = piece_bitboards[side][BISHOP] | piece_bitboards[side][QUEEN];
    if (get_bishop_attacks(sq, occupancies[BOTH]) & bishop_eq) return true;

    U64 rook_eq = piece_bitboards[side][ROOK] | piece_bitboards[side][QUEEN];
    if (get_rook_attacks(sq, occupancies[BOTH]) & rook_eq) return true;

    return false;
}