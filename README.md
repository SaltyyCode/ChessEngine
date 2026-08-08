# SaltyyBot ♟️
 
Un moteur d'échecs en C++ compatible UCI, basé sur des **bitboards** et des **magic bitboards** pour la génération de coups des pièces glissantes (fou, tour, dame).
 
## Fonctionnalités
 
- Représentation du plateau par bitboards (`U64`, un par couleur/pièce)
- Génération de coups légers (pions, cavaliers, roi) et glissants (fou, tour, dame) via **magic bitboards**
- Gestion complète des règles spéciales : roque, prise en passant, promotions (y compris promotion + capture)
- `make_move` / `unmake_move` avec historique d'état (droits de roque, en passant, pièce capturée)
- Détection de case attaquée (`is_square_attacked`) pour la légalité des coups et le roque
- Recherche **alpha-bêta** avec quiescence search (captures)
- Évaluation matérielle + tables positionnelles (PST) par pièce, bonus de roque
- Interface **UCI** minimale (`uci`, `isready`, `ucinewgame`, `position`, `go`)
- Outils **perft** / **perft divide** pour valider la génération de coups
## Structure du projet
 
```
Board/
  attacks.hpp / attacks.cpp   → tables d'attaques (leapers + sliders via magic bitboards)
  board.hpp   / board.cpp     → représentation, parsing FEN, affichage
  move.hpp    / move.cpp      → encodage des coups (16 bits), make/unmake
  movegen.hpp / movegen.cpp   → génération de tous les coups pseudo-légaux
  magics.hpp                  → nombres magiques précalculés + bits pertinents
  perft.hpp   / perft.cpp     → perft et perft divide
Eval/
  eval.hpp    / eval.cpp      → évaluation matérielle + PST
Search/
  search.hpp  / search.cpp    → alpha-bêta + quiescence search
Uci/
  uci.hpp     / uci.cpp       → boucle UCI
main.cpp                      → point d'entrée / tests manuels
```
 
## Encodage des coups
 
Un `Move` tient sur 16 bits :
 
| Bits    | Contenu                  |
|---------|---------------------------|
| 0–5     | Case de départ (0–63)     |
| 6–11    | Case d'arrivée (0–63)     |
| 12–15   | Drapeaux (type / promotion) |
 
Les drapeaux de promotion encodent la pièce sur les 2 bits de poids faible (0=cavalier, 1=fou, 2=tour, 3=dame), avec ou sans capture (bit 2), ce qui simplifie leur décodage.
 
## Compilation
 
```bash
g++ -O2 -std=c++17 -I. main.cpp Board/*.cpp Eval/*.cpp Search/*.cpp Uci/*.cpp -o saltyybot
```

 
## Validation (perft)
 
La génération de coups a été validée par perft depuis la position de départ et Kiwipete :
 
| Position         | Profondeur | Résultat attendu |
|-------------------|------------|-------------------|
| Position initiale | 6          | 119 060 324       |
| Kiwipete           | 5          | 193 690 690       |
 
```
position startpos
go perft 6
```
