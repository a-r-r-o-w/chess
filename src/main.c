#include "stdio.h"

#include "chess/chess.h"
#include "chess/fen.h"
#include "gui/chess_gui.h"

int main (int argc, char* argv[]) {
    Chess chess;
    Chess_Constructor(&chess);
    ParseFEN(&chess, argv[1]);
    
    ChessGUI(&chess);

    return 0;
}
