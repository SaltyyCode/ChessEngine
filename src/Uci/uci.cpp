#include "Uci/uci.hpp"
#include "Board/board.hpp"
#include "Board/movegen.hpp"
#include "Search/search.hpp"
#include <iostream>
#include <sstream>
#include <vector>

static std::string move_to_uci(Move move)
{
    if (move == 0) return "0000";

    int src = get_move_source(move);
    int tgt = get_move_target(move);
    int flags = get_move_flags(move);

    std::string res = "";
    res += (char)('a' + (src % 8));
    res += (char)('1' + (src / 8));
    res += (char)('a' + (tgt % 8));
    res += (char)('1' + (tgt / 8));

    if (flags == FLAG_PROMOTION_QUEEN)  res += "q";
    if (flags == FLAG_PROMOTION_ROOK)   res += "r";
    if (flags == FLAG_PROMOTION_BISHOP) res += "b";
    if (flags == FLAG_PROMOTION_KNIGHT) res += "n";

    return res;
}

UciHandler::UciHandler()
{
    commands_["uci"]        = [this](auto& s) { handle_uci(s); };
    commands_["isready"]    = [this](auto& s) { handle_isready(s); };
    commands_["ucinewgame"] = [this](auto& s) { handle_ucinewgame(s); };
    commands_["position"]   = [this](auto& s) { handle_position(s); };
    commands_["go"]         = [this](auto& s) { handle_go(s); };
}

void UciHandler::loop()
{
    std::string line, cmd;

    while (std::getline(std::cin, line)) {
        if (line == "quit")
            break;

        std::istringstream stream(line);
        stream >> cmd;
        if (commands_.count(cmd))
            commands_[cmd](stream);
    }
}

void UciHandler::handle_uci(std::istringstream& stream)
{
    std::cout << "id name SaltyyBot\n";
    std::cout << "id author Mighty Saltyy\n";
    std::cout << "uciok\n" << std::flush;
}

void UciHandler::handle_isready(std::istringstream& stream)
{
    std::cout << "readyok\n" << std::flush;
}

void UciHandler::handle_ucinewgame(std::istringstream& stream)
{
    board_.reset();
}

void UciHandler::handle_position(std::istringstream& stream)
{
    std::string type;
    stream >> type;

    if (type == "startpos") {
        board_.parse_fen(START_FEN);
    } else if (type == "fen") {
        std::string fen_part, fen;
        for (int i = 0; i < 6 && stream >> fen_part; ++i) {
            fen += fen_part + (i < 5 ? " " : "");
        }
        board_.parse_fen(fen);
    }

    std::string moves_token;
    if (stream >> moves_token && moves_token == "moves") {
        std::string move_str;
        while (stream >> move_str) {
            std::vector<Move> move_list;
            generate_all_moves(board_, move_list);

            for (Move m : move_list) {
                if (move_to_uci(m) == move_str) {
                    board_.make_move(m);
                    break;
                }
            }
        }
    }
}

void UciHandler::handle_go(std::istringstream& stream)
{
    std::string token;
    int depth = 5;

    while (stream >> token) {
        if (token == "depth") {
            stream >> depth;
        }
    }

    Move best = search_best_move(board_, depth);
    std::cout << "bestmove " << move_to_uci(best) << "\n" << std::flush;
}