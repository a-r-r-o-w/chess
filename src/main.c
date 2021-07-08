#include "stdio.h"

#include "chess/chess.h"
#include "chess/fen.h"
#include "gui/chess_gui.h"

int main (int argc, char* argv[]) {
    Chess chess;
    Chess_Constructor(&chess);
    ParseFEN(&chess, "rnbqkbnr/ppp2ppp/4p3/3pP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 3");
    
    ChessGUI(&chess);

    // Chess chess;
    // Chess_Constructor(&chess);
    // ParseFEN(&chess, argv[1]);

    // for (int i = 0; i < 16; ++i) {
    //     vector_int valid;
    //     vector_int_constructor(&valid, 0, 0);

    //     if (chess.m_White[i].m_Type == white_knight) {
    //         generateKnightMoves(&chess, chess.m_White + i, &valid);
    //         for (int j = 0; j < valid.size; ++j)
    //             printf("%c%c\n", valid.data[j] % 8 + 'a', 7 - valid.data[j] / 8 + '1');
    //     }

    //     vector_int_destructor(&valid);
    // }

    return 0;
}
