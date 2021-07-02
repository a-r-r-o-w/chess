#ifndef chess_h
#define chess_h

#include "stdlib.h"

#include "piece.h"

typedef struct Chess Chess;

#define maxPieces 16

/* Function declarations ------------------------------------------------------------------ */

void Chess_Constructor (Chess*);

/* Object types --------------------------------------------------------------------------- */

struct Chess {
    Piece m_White[maxPieces];
    Piece m_Black[maxPieces];

    bool m_IsWhiteTurn;
    bool m_CanWhiteCastleK;
    bool m_CanWhiteCastleQ;
    bool m_CanBlackCastleK;
    bool m_CanBlackCastleQ;

    int m_enPassantTarget;
    int m_halfMoves;
    int m_fullMoves;
};

/* Function definitions ------------------------------------------------------------------- */

void Chess_Constructor (Chess* chess) {
    for (int i = 0; i < maxPieces; ++i) {
        Piece_Constructor(chess->m_White + i);
        Piece_Constructor(chess->m_Black + i);
    }

    chess->m_IsWhiteTurn     = false;
    chess->m_CanWhiteCastleK = false;
    chess->m_CanWhiteCastleQ = false;
    chess->m_CanBlackCastleK = false;
    chess->m_CanBlackCastleQ = false;

    chess->m_enPassantTarget = -1;
    chess->m_halfMoves       = 0;
    chess->m_fullMoves       = 0;
}

#endif // chess_h
