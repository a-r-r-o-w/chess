#ifndef chess_pieces_h
#define chess_pieces_h

#include "stdint.h"

#include "exceptions.h"

typedef uint64_t position;

int count_trailing_zeroes (position position) {
    int ctz = 0;
    for (int bit = 0; bit < 64; ++bit, ++ctz)
        if (position & (1ull << bit))
            break;
    return ctz;
}

typedef struct {
    char symbol;
    position position;
} piece;

void set_piece (piece* piece, char coordinate[2], char symbol) {
    piece->position = 0;
    piece->symbol = '*';

    int file = coordinate[0] - 'a';
    int rank = coordinate[1] - '1';

    int shift = 8 * (8 - rank - 1) + file;

    // number of possible positions on a chess board are 64
    // we index them from [0, 1, ..., 63]
    if (shift > (64 - 1)) {
        exception("invalid position provided", __FILE__, __LINE__);
    }

    uint64_t mask = 1;
    mask <<= shift;
    piece->position |= mask;
    piece->symbol = symbol;
}

char* get_position (piece* piece) {
    int ctz = count_trailing_zeroes(piece->position);

    char rank = 7 - ctz / 8;
    char file = ctz % 8;
    
    rank += '1';
    file += 'a';

    return (char[2]){file, rank};
}

#endif
