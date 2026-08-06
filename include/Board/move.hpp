#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>
#include <string>

/*
  Découpage d'un Move (16 bits) :
  
  0000 0000 0011 1111  ->  Bits 0 à 5   : Case de départ (0 à 63)
  0000 1111 1100 0000  ->  Bits 6 à 11  : Case d'arrivée (0 à 63)
  1111 0000 0000 0000  ->  Bits 12 à 15 : Drapeaux (Type de coup / Promotion)
*/

using Move = std::uint16_t;

// Masque pour isoler 6 bits (0x3F = 00111111 en binaire)
constexpr std::uint16_t MOVE_MASK = 0x3F;

enum MoveFlags {
    FLAG_QUIET          = 0,
    FLAG_DOUBLE_PUSH    = 1,
    FLAG_KING_CASTLE    = 2,
    FLAG_QUEEN_CASTLE   = 3,
    FLAG_CAPTURE        = 4,
    FLAG_EN_PASSANT     = 5,
    
    FLAG_PROMO_KNIGHT   = 8,
    FLAG_PROMO_BISHOP   = 9,
    FLAG_PROMO_ROOK     = 10,
    FLAG_PROMO_QUEEN    = 11,
    
    FLAG_PROMO_CAP_KNIGHT = 12,
    FLAG_PROMO_CAP_BISHOP = 13,
    FLAG_PROMO_CAP_ROOK   = 14,
    FLAG_PROMO_CAP_QUEEN  = 15
};

inline Move encode_move(int source, int target, int flags) {
    return (source & MOVE_MASK) | 
          ((target & MOVE_MASK) << 6) | 
          ((flags & 0x0F) << 12);
}

inline int get_move_source(Move move) { return move & MOVE_MASK; }
inline int get_move_target(Move move) { return (move >> 6) & MOVE_MASK; }
inline int get_move_flags(Move move)  { return (move >> 12) & 0x0F; }

inline bool is_capture(Move move) {
    return (get_move_flags(move) & 4) != 0;
}

inline bool is_promotion(Move move) {
    return (get_move_flags(move) & 8) != 0;
}

#endif