#include "stdio.h"

#include "chess.h"

int main (int argc, char* argv[]) {
    // "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

    struct chess chess;
    chess_constructor(&chess);
    set_board_position(&chess, argv[1]);
    display_board(&chess.board, true);
    puts("");
    display_info(&chess);

    return 0;
}