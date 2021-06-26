#ifndef chess_board_h
#define chess_board_h

#include "stdbool.h"

#include "types.h"
#include "piece.h"

void board_constructor (struct board*);
void set_board_position (struct chess*, const char*);

struct board {
    struct piece white[16];
    struct piece black[16];

    int is_occupied[64];
};

void board_constructor (struct board* board) {
    for (int i = 0; i < 16; ++i) {
        piece_constructor(&board->white[i]);
        piece_constructor(&board->black[i]);
    }
    for (int i = 0; i < 64; ++i)
        board->is_occupied[i] = 0;
}

void display_board (struct board* board, bool is_white_perspective) {
    char pretty_board [64];

    for (int i = 0; i < 64; ++i)
        pretty_board[i] = '-';

    for (int i = 0; i < 16; ++i) {
        int index = board->white[i].position;
        enum piece_type type = board->white[i].type;
        
        if (index == -1)
            continue;
        
        pretty_board[index] = __symbol[type];
    }

    for (int i = 0; i < 16; ++i) {
        int index = board->black[i].position;
        enum piece_type type = board->black[i].type;
        
        if (index == -1)
            continue;
        
        pretty_board[index] = __symbol[type];
    }

    if (is_white_perspective) {
        for (int i = 0; i < 64; ++i) {
            printf("%c ", pretty_board[i]);
            if ((i + 1) % 8 == 0)
                printf("\n");
        }
    }
    else {
        for (int i = 64 - 1; i >= 0; --i) {
            printf("%c ", pretty_board[i]);
            if (i % 8 == 0)
                printf("\n");
        }
    }
}

#endif // chess_board_h
