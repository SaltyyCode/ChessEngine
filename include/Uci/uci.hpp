#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>

class UciHandler {
public:
    UciHandler() {
        commands_["uci"]        = [this](auto& stream) { handle_uci(stream); };
        commands_["isready"]    = [this](auto& stream) { handle_isready(stream); };
        commands_["ucinewgame"] = [this](auto& stream) { handle_ucinewgame(stream); };
        commands_["position"]   = [this](auto& stream) { handle_position(stream); };
        commands_["go"]         = [this](auto& stream) { handle_go(stream); };
    }
