#ifndef UCI_HPP
#define UCI_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <functional>

class UciHandler {
public:
    UciHandler();
    void loop();

private:
    std::unordered_map<std::string, std::function<void(std::istringstream&)>> commands_;

    void handle_uci(std::istringstream& stream);
    void handle_isready(std::istringstream& stream);
    void handle_ucinewgame(std::istringstream& stream);
    void handle_position(std::istringstream& stream);
    void handle_go(std::istringstream& stream);
};

#endif