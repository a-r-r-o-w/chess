/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#include "stdio.h"

#include "chess/chess.h"
#include "chess/fen.h"
#include "gui/chess_gui.h"

int main (int argc, char* argv[]) {
    const char* beginningPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Chess chess;
    Chess_Constructor(&chess);

    ParseFEN(&chess, beginningPosition);

    system("cls");

    printf("Chess\n"
           "\n"
           "Written by:\n"
           "  Aryan V S         (PES1UG20CS083)\n"
           "  Aryansh Bhargavan (PES1UG20CS084)\n"
           "  Avanish Bhat      (PES1UG20CS092)\n"
           "\n"
           "\n"
           "Press any key to start GUI");
    
    getchar();

    system("cls");

    ChessGUI(&chess);

    return 0;
}
