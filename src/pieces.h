#ifndef chess_pieces_h
#define chess_pieces_h

#include "stdint.h"

#include "exceptions.h"

typedef uint64_t position;

typedef struct {
    char symbol;
    position position;
} piece;

void piece_constructor (piece* piece, char symbol) {
    piece->position = 0;
    piece->symbol = symbol;
}

void piece_set (piece* piece, char file, char rank) {
    file -= 'a';
    rank -= '1';

    int shift = 8 * (8 - rank - 1) + file;

    // number of possible positions on a chess board are 64
    // we index them from [0, 1, ..., 63]
    if (shift > (64 - 1)) {
        exception("invalid position provided", __FILE__, __LINE__);
    }

    uint64_t mask = 1;
    mask <<= shift;
    piece->position |= mask;
}

#endif
