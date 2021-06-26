#include "stdio.h"

#include "chess.h"
#include "move_generation.h"

int main (int argc, char* argv[]) {
    // const char* FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    struct chess chess;
    chess_constructor(&chess);
    set_board_position(&chess, argv[1]);
    display_board(&chess.board, true);
    puts("");
    display_info(&chess);

    vector_position v;
    vector_position_constructor(&v, 0, 0);
    __generate_sliding_moves(&chess, &chess.board.white[1], &v);

    for (int i = 0; i < v.size; ++i)
        printf("%c%c\n", v.data[i] % 8 + 'a', 7 - v.data[i] / 8 + '1');
    
    // for (int i = 0; i < 64; ++i) {
    //     printf("%5d", chess.board.is_occupied[i]);
    //     if ((i + 1) % 8 == 0)
    //         puts("");
    // }

    vector_position_destructor(&v);

    return 0;
}
