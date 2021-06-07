#include "stdio.h"

#include "board.h"

int main () {
    board board;
    board_constructor(&board);
    display_board(&board);

    return 0;
}