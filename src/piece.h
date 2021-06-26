#ifndef chess_piece_h
#define chess_piece_h

#include "stdbool.h"

#include "types.h"

void piece_constructor (struct piece*);
void set_piece_position (struct piece*, char, char);
int __indexify (char, char);
bool __is_white_piece (struct piece*);
bool __is_black_piece (struct piece*);

struct piece {
    position position;

    enum piece_type type;
};

void piece_constructor (struct piece* piece) {
    piece->position = -1;
    piece->type = none;
}

void set_piece (struct piece* piece, char file, char rank, enum piece_type piece_type) {
    piece->position = __indexify(file, rank);
    piece->type = piece_type;
}

int __indexify (char file, char rank) {
    return 8 * ('8' - rank) + file - 'a';
}

bool __is_white_piece (struct piece* piece) {
    return
        piece->type == white_king   ||
        piece->type == white_queen  ||
        piece->type == white_rook   ||
        piece->type == white_bishop ||
        piece->type == white_knight ||
        piece->type == white_pawn;
}

bool __is_black_piece (struct piece* piece) {
    return
        piece->type == black_king   ||
        piece->type == black_queen  ||
        piece->type == black_rook   ||
        piece->type == black_bishop ||
        piece->type == black_knight ||
        piece->type == black_pawn;
}

#endif // chess_piece_h
