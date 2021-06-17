#ifndef chess_board_h
#define chess_board_h

#include "types.h"
#include "piece.h"

void board_constructor (struct board*);
void __construct_pieces (struct piece*, int, int, enum piece_type);
void set_board_position (struct chess*, const char*);

struct board {
    struct piece white[16];
    struct piece black[16];
};

void board_constructor (struct board* board) {
    __construct_pieces(board->white, 0, 1, white_king);
    __construct_pieces(board->white, 1, 1, white_queen);
    __construct_pieces(board->white, 2, 2, white_rook);
    __construct_pieces(board->white, 4, 2, white_bishop);
    __construct_pieces(board->white, 6, 2, white_knight);
    __construct_pieces(board->white, 8, 8, white_pawn);

    __construct_pieces(board->black, 0, 1, black_king);
    __construct_pieces(board->black, 1, 1, black_queen);
    __construct_pieces(board->black, 2, 2, black_rook);
    __construct_pieces(board->black, 4, 2, black_bishop);
    __construct_pieces(board->black, 6, 2, black_knight);
    __construct_pieces(board->black, 8, 8, black_pawn);
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

void __construct_pieces (struct piece* pieces, int startpos, int count, enum piece_type type) {
    for (int i = 0; i < count; ++i)
        piece_constructor(&pieces[startpos + i], type);
}

#endif // chess_board_h
