#include "Uci/uci.hpp"

UciHandler::UciHandler() {
    commands_["uci"]        = [this](auto& s) { handle_uci(s); };
    commands_["isready"]    = [this](auto& s) { handle_isready(s); };
    commands_["ucinewgame"] = [this](auto& s) { handle_ucinewgame(s); };
    commands_["position"]   = [this](auto& s) { handle_position(s); };
    commands_["go"]         = [this](auto& s) { handle_go(s); };
}

void UciHandler::loop()
{
    std::string line, cmd;

    while(std::getline(std::cin, line)){
        if(line == "quit")
            break;
    
        std::istringstream stream(line);
        stream >> cmd;
        if(commands_.count(cmd))
            commands_[cmd](stream);
    }
}

void UciHandler::handle_uci(std::istringstream& stream)
{
    std::cout << "id name SaltyyBot" << std::endl;
    std::cout << "id author Mighty Saltyy" << std::endl;
    std::cout << "uciok" << std::endl;
    //std::cout << options : ... << std::endl (a faire plus tard);
}

void UciHandler::handle_isready(std::istringstream& stream)
{
    std::cout << "readyok" << std::endl;
}

void UciHandler::handle_ucinewgame(std::istringstream& stream)
{
    //Reinitialisation du jeu
}

void UciHandler::handle_position(std::istringstream& stream)
{
    //Parsing de la position
}

void UciHandler::handle_go(std::istringstream& stream)
{
    // Recherche et envoi du meilleur coup
    std::cout << "bestmove e2e4" << std::endl;
}

int main() {
    UciHandler uci;
    uci.loop();
    return 0;
}
