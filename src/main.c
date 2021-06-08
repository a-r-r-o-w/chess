#include "stdio.h"
#include "stdbool.h"

#include "board.h"

int main (int argc, char* argv[]) {
    // const char* FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    board board;
    board_constructor(&board, argv[1]);
    
    printf("\nwhite:\n");
    display_board(&board, true);
    printf("\nblack\n");
    display_board(&board, false);

    return 0;
}