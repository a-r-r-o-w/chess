/**
 * Chess
 * 
 * Written by:
 *    Aryan V S         (PES1UG20CS083)
 *    Aryansh Bhargavan (PES1UG20CS084)
 *    Avanish Bhat      (PES1UG20CS092)
 */

#ifndef chess_constants_h
#define chess_constants_h

enum PieceType;
typedef enum PieceType PieceType;

enum PieceType {
    white_king,
    white_queen,
    white_rook,
    white_bishop,
    white_knight,
    white_pawn,
    black_king,
    black_queen,
    black_rook,
    black_bishop,
    black_knight,
    black_pawn,
    none
};

#endif // chess_constants_h
