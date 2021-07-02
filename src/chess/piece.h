#ifndef chess_piece_h
#define chess_piece_h

#include "stdbool.h"

#include "constants.h"

typedef struct Piece Piece;

/* Function declarations ------------------------------------------------------------------ */

void Piece_Constructor (Piece*);
void Piece_Set         (Piece*, char, char, PieceType);

int indexify (char, char);

/* Object types --------------------------------------------------------------------------- */

struct Piece {
    int       m_Position;
    PieceType m_Type;
};

/* Function definitions ------------------------------------------------------------------- */

void Piece_Constructor (Piece* piece) {
    piece->m_Position = -1;
    piece->m_Type     = none;
}

void Piece_Set (Piece* piece, char file, char rank, PieceType type) {
    piece->m_Position = indexify(file, rank);
    piece->m_Type     = type;
}

int indexify (char file, char rank)
{ return 8 * ('8' - rank) + file - 'a'; }

#endif // chess_piece_h
