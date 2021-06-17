#ifndef chess_piece_h
#define chess_piece_h

#include "stdbool.h"

#include "types.h"

void piece_constructor (struct piece*, enum piece_type);
void set_piece_position (struct piece*, char, char);
int __indexify (char, char);

struct piece {
    position position;

    enum piece_type type;
};

void piece_constructor (struct piece* piece, enum piece_type type) {
    piece->position = -1;
    piece->type = type;
}

void set_piece_position (struct piece* piece, char file, char rank) {
    piece->position = __indexify(file, rank);
}

int __indexify (char file, char rank) {
    return 8 * ('8' - rank) + file - 'a';
}

#endif // chess_piece_h
