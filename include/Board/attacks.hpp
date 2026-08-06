#ifndef ATTACKS_HPP
#define ATTACKS_HPP

#include <cstdint>

using U64 = std::uint64_t;

// Masques de colonnes pour empêcher le chevauchement (wrap-around)
constexpr U64 NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL; // Efface la colonne A
constexpr U64 NOT_B_FILE  = 0xFCFCFCFCFCFCFCFCULL; // Efface les colonnes A et B
constexpr U64 NOT_G_FILE  = 0x3F3F3F3F3F3F3F3FULL; // Efface les colonnes G et H
constexpr U64 NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL; // Efface la colonne H

extern U64 pawn_attacks[2][64];
extern U64 knight_attacks[64];
extern U64 king_attacks[64];

extern U64 bishop_masks[64];
extern U64 rook_masks[64];

void init_leaper_attacks();
void init_sliders_attacks(bool is_bishop);
void init_all_attacks();

U64 mask_pawn_attacks(int side, int sq);
U64 mask_knight_attacks(int sq);
U64 mask_king_attacks(int sq);
U64 mask_bishop_attacks(int sq);
U64 mask_rook_attacks(int sq);
U64 bishop_attacks_on_the_fly(int sq, U64 block);
U64 rook_attacks_on_the_fly(int sq, U64 block);
U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

U64 get_bishop_attacks(int sq, U64 occupancy);
U64 get_rook_attacks(int sq, U64 occupancy);
U64 get_queen_attacks(int sq, U64 occupancy);

#endif