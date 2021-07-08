/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_h
#define chess_h

#include "stdlib.h"

#include "piece.h"

typedef struct Chess Chess;

#define maxPieces 16

/* Function declarations ------------------------------------------------------------------ */

void Chess_Constructor     (Chess*);
void Chess_CopyConstructor (Chess*, Chess*);

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

void Chess_CopyConstructor (Chess* lhs, Chess* rhs) {
    for (int i = 0; i < 16; ++i) {
        lhs->m_White[i] = rhs->m_White[i];
        lhs->m_Black[i] = rhs->m_Black[i];
    }
    lhs->m_IsWhiteTurn     = rhs->m_IsWhiteTurn;
    lhs->m_CanWhiteCastleK = rhs->m_CanWhiteCastleK;
    lhs->m_CanWhiteCastleQ = rhs->m_CanWhiteCastleQ;
    lhs->m_CanBlackCastleK = rhs->m_CanBlackCastleK;
    lhs->m_CanBlackCastleQ = rhs->m_CanBlackCastleQ;
    lhs->m_enPassantTarget = rhs->m_enPassantTarget;
    lhs->m_halfMoves       = rhs->m_halfMoves;
    lhs->m_fullMoves       = rhs->m_fullMoves;
}

#endif // chess_h
