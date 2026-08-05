#ifndef BOARD_HPP
#define BOARD_HPP

#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>
#include <cctype>

using U64 = std::uint64_t;

const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
enum Color { WHITE, BLACK, BOTH };
enum Piece { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

enum Square {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8, SQ_NONE
};

enum Castling { WK = 1, WQ = 2, BK = 4, BQ = 8 };

inline void set_bit(U64& bb, int sq) { bb |= (1ULL << sq); }
inline bool get_bit(U64 bb, int sq) { return (bb >> sq) & 1ULL; }
inline void pop_bit(U64& bb, int sq) { bb &= ~(1ULL << sq); }

void print_bitboard(U64 bb);

class Board {
public:
    Board();

    U64 piece_bitboards[2][6];
    U64 occupancies[3];

    Color side_to_move;
    Square en_passant;
    uint8_t castling_rights;

    void reset();
    void update_occupancies();
    void print() const;
    void parse_fen(const std::string& fen);

private:
    void parse_pieces(const std::string& placement);
    void parse_state(const std::string& side, const std::string& castling, const std::string& ep);
};

#endif