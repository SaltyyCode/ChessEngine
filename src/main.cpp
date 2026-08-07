#include "Board/board.hpp"
#include "Board/attacks.hpp"
#include <iostream>

int main() {
 
    init_all_attacks();

    Board board;


    board.parse_fen("4k3/8/8/8/4q3/8/8/4K3 w - - 0 1");
    std::cout << "=== TEST 1 : Dame noire en e4 vers Roi en e1 ===";
    board.print();
    bool is_e1_attacked = board.is_square_attacked(E1, BLACK);
    std::cout << "Case e1-attaquée par les Noirs ? " 
              << (is_e1_attacked ? "OUI (Échec au Roi !)" : "NON") << "\n\n";


    board.parse_fen("4k3/8/8/8/4q3/8/4P3/4K3 w - - 0 1");
    std::cout << "=== TEST 2 : Dame e4 bloquée par le Pion e2 ===";
    board.print();
    is_e1_attacked = board.is_square_attacked(E1, BLACK);
    std::cout << "Case e1 attaquée par les Noirs ? " 
              << (is_e1_attacked ? "OUI" : "NON (Le pion e2 bloque le rayon !)") << "\n\n";


    board.parse_fen("4k3/8/8/8/8/8/2n5/4K3 w - - 0 1");
    std::cout << "=== TEST 3 : Cavalier noir en c2 ===";
    board.print();
    is_e1_attacked = board.is_square_attacked(E1, BLACK);
    std::cout << "Case e1 attaquée par les Noirs ? " 
              << (is_e1_attacked ? "OUI (Échec au Roi !)" : "NON") << "\n\n";

    return 0;
}