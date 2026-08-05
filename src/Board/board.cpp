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
                    symbol = "PNBRQK"[p];
                } else if (get_bit(piece_bitboards[BLACK][p], sq)) {
                    symbol = "pnbrqk"[p];
                }
            }
            std::cout << symbol << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
}

void Board::parse_pieces(const std::string& placement)
{
    int rank = 7;
    int file = 0;

    for (char c : placement) {
        if (c == '/') {
            rank--;
            file = 0;
        } else if (c >= '1' && c <= '8') {
            file += (c - '0');
        } else {
            int sq = rank * 8 + file;
            Color color = std::isupper(c) ? WHITE : BLACK;
            Piece piece;

            switch (std::tolower(c)) {
                case 'p': piece = PAWN; break;
                case 'n': piece = KNIGHT; break;
                case 'b': piece = BISHOP; break;
                case 'r': piece = ROOK; break;
                case 'q': piece = QUEEN; break;
                case 'k': piece = KING; break;
                default: continue;
            }

            set_bit(piece_bitboards[color][piece], sq);
            file++;
        }
    }
}

void Board::parse_state(const std::string& side, const std::string& castling, const std::string& ep)
{
    side_to_move = (side == "w") ? WHITE : BLACK;
    castling_rights = 0;

    for (char c : castling) {
        if (c == 'K') castling_rights |= WK;
        if (c == 'Q') castling_rights |= WQ;
        if (c == 'k') castling_rights |= BK;
        if (c == 'q') castling_rights |= BQ;
    }

    if (ep != "-") {
        int f = ep[0] - 'a';
        int r = ep[1] - '1';
        en_passant = static_cast<Square>(r * 8 + f);
    } else {
        en_passant = SQ_NONE;
    }
}

void Board::parse_fen(const std::string& fen)
{
    reset();
    std::istringstream stream(fen);
    std::string placement, side, castling, ep;

    stream >> placement >> side >> castling >> ep;

    parse_pieces(placement);
    parse_state(side, castling, ep);
    update_occupancies();
}