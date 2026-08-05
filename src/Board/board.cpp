#include "Board/board.hpp"
#include <iostream>

void print_bitboard(U64 bb)
{
    std::cout << "\n";
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            std::cout << (get_bit(bb, sq) ? "1 " : ". ");
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
    std::cout << "  Valeur hexa : 0x" << std::hex << bb << std::dec << "\n\n";
}

Board::Board()
{
    reset();
}

void Board::update_occupancies()
{
    occupancies[WHITE] = 0ULL;
    occupancies[BLACK] = 0ULL;
    occupancies[BOTH]  = 0ULL;

    for (int piece = PAWN; piece <= KING; ++piece) {
        occupancies[WHITE] |= piece_bitboards[WHITE][piece];
        occupancies[BLACK] |= piece_bitboards[BLACK][piece];
    }
    occupancies[BOTH] = occupancies[WHITE] | occupancies[BLACK];
}

void Board::reset()
{
    for (int c = 0; c < 2; ++c) {
        for (int p = 0; p < 6; ++p) {
            piece_bitboards[c][p] = 0ULL;
        }
    }

    side_to_move = WHITE;
    en_passant = SQ_NONE;
    castling_rights = 0;
    update_occupancies();
}

void Board::print() const
{
    std::cout << "\n";
    for (int rank = 7; rank >= 0; --rank) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; ++file) {
            int sq = rank * 8 + file;
            char symbol = '.';

            for (int p = PAWN; p <= KING; ++p) {
                if (get_bit(piece_bitboards[WHITE][p], sq)) {
                    symbol = "PBNRQK"[p];
                } else if (get_bit(piece_bitboards[BLACK][p], sq)) {
                    symbol = "pbnrqk"[p];
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
}