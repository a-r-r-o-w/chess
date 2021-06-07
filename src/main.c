#include "stdio.h"

#include "board.h"

int main () {
    const char* FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    board board;
    board_constructor(&board, FEN);
    display_board(&board);

    return 0;
}