/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

    #ifndef chess_piece_h
#define chess_piece_h

#include "stdbool.h"

#include "constants.h"

typedef struct Piece Piece;

#define isPieceType(x)                   \
bool is_##x (Piece* piece) {             \
    return piece->m_Type == white_##x || \
           piece->m_Type == black_##x;   \
}

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

isPieceType(king)
isPieceType(queen)
isPieceType(rook)
isPieceType(bishop)
isPieceType(knight)
isPieceType(pawn)

bool isWhitePiece (Piece* piece) {
    return piece->m_Type == white_king   ||
           piece->m_Type == white_queen  ||
           piece->m_Type == white_rook   ||
           piece->m_Type == white_bishop ||
           piece->m_Type == white_knight ||
           piece->m_Type == white_pawn;
}

bool isBlackPiece (Piece* piece) {
    return piece->m_Type == black_king   ||
           piece->m_Type == black_queen  ||
           piece->m_Type == black_rook   ||
           piece->m_Type == black_bishop ||
           piece->m_Type == black_knight ||
           piece->m_Type == black_pawn;
}

#endif // chess_piece_h
