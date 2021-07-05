#ifndef chess_move_generation_h
#define chess_move_gereration_h

#include "stdbool.h"

#include "exception.h"
#include "chess.h"
#include "piece.h"
#include "vector.h"

/* Function declarations ------------------------------------------------------------------ */

void generateMoves           (Chess*, Piece*, vector_int*);
void generateSlidingMoves    (Chess*, Piece*, vector_int*);
void generateKnightMoves     (Chess*, Piece*, vector_int*);
void generatePawnMoves_white (Chess*, Piece*, vector_int*);
void generatePawnMoves_black (Chess*, Piece*, vector_int*);
bool isOccupiedByWhite (Chess*, int);
bool isOccupiedByBlack (Chess*, int);

/* Object types --------------------------------------------------------------------------- */

/* Function definitions ------------------------------------------------------------------- */

void generateMoves (Chess* chess, Piece* piece, vector_int* valid) {
    if (is_pawn(piece)) {
        if (isWhitePiece(piece))
            generatePawnMoves_white(chess, piece, valid);
        else
            generatePawnMoves_black(chess, piece, valid);
    }
    else if (is_knight(piece))
        generateKnightMoves(chess, piece, valid);
    else
        generateSlidingMoves(chess, piece, valid);
}

void generateSlidingMoves (Chess* chess, Piece* piece, vector_int* valid) {
    int directions[] = {
        -1, // left
        +1, // right
        -8, // up
        +8, // down
        -9, // up-left
        -7, // up-right
        +7, // down-left
        +9  // down-right
    };

    if (piece->m_Position == -1)
        return;
    
    int begin = 0;
    int end   = 8;
    int count = 8;

    if (is_king(piece))
        count = 1;
    else if (is_queen(piece))
        ;
    else if (is_rook(piece))
        end = 4;
    else if (is_bishop(piece))
        begin = 4;
    else
        EXCEPTION("Move generation for this piece type is not supported");
    
    for (int i = begin; i < end; ++i) {
        for (int times = 0; times < count; ++times) {
            int newPosition = piece->m_Position + (times + 1) * directions[i];

            if (newPosition < 0 || newPosition >= 64)
                continue;

            char file = newPosition % 8 + 'a';
            char rank = 8 - newPosition / 8 - 1 + '1';

            if ((isWhitePiece(piece) && isOccupiedByWhite(chess, newPosition)) ||
                (isBlackPiece(piece) && isOccupiedByBlack(chess, newPosition)))
                break;

            vector_int_pushback(valid, newPosition);

            if ((isWhitePiece(piece) && isOccupiedByBlack(chess, newPosition)) ||
                (isBlackPiece(piece) && isOccupiedByWhite(chess, newPosition)))
                break;

            if ((i == 0 && file == 'a') ||
                (i == 1 && file == 'h') ||
                (i == 2 && rank == '8') ||
                (i == 3 && rank == '1') ||
                (i == 4 && (file == 'a' || rank == '8')) ||
                (i == 5 && (file == 'h' || rank == '8')) ||
                (i == 6 && (file == 'a' || rank == '1')) ||
                (i == 7 && (file == 'h' || rank == '1')))
                break;
        }
    }
}

void generateKnightMoves (Chess* chess, Piece* piece, vector_int* valid) {
    int primaryDirection[] = {
        -1, // left
        +1, // right
        -8, // up
        +8  // down
    };

    int secondaryDirection[][2] = {
        {-8, +8}, // up, down
        {-8, +8}, // up, down
        {-1, +1}, // left, right
        {-1, +1}  // left, right
    };

    if (!is_knight(piece))
        EXCEPTION("Move generation for this piece type is not supported");

    char file = piece->m_Position % 8 + 'a';
    char rank = 8 - piece->m_Position / 8 - 1 + '1';
    
    for (int i = 0; i < 4; ++i) {

        if ((i == 0 && file < 'c') ||
            (i == 1 && file > 'f') ||
            (i == 2 && rank > '6') ||
            (i == 3 && rank < '3'))
            continue;
        
        for (int j = 0; j < 2; ++j) {
            if ((i == 0 && ((rank == '1' && j == 1) || (rank == '8' && j == 0))) ||
                (i == 1 && ((rank == '1' && j == 1) || (rank == '8' && j == 0))) ||
                (i == 2 && ((file == 'a' && j == 0) || (file == 'h' && j == 1))) ||
                (i == 3 && ((file == 'a' && j == 0) || (file == 'h' && j == 1))))
                continue;

            int newPosition = piece->m_Position + 2 * primaryDirection[i] + secondaryDirection[i][j];

            if (newPosition < 0 || newPosition >= 64)
                continue;
            
            if ((isWhitePiece(piece) && isOccupiedByWhite(chess, newPosition)) ||
                (isBlackPiece(piece) && isOccupiedByBlack(chess, newPosition)))
                continue;
            
            vector_int_pushback(valid, newPosition);
        }
    }
}

void generatePawnMoves_white (Chess* chess, Piece* piece, vector_int* valid) {
    int direction[] = {
        -8, // up
        -9, // up-left
        -7  // up-right
    };

    char file = piece->m_Position % 8 + 'a';
    char rank = 8 - piece->m_Position / 8 - 1 + '1';

    for (int i = 0; i < 3; ++i) {
        if (rank == '8')
            EXCEPTION("white pawn cannot exist on the 8th rank");
        
        if ((i == 1 && file == 'a') ||
            (i == 2 && file == 'h'))
            continue;
        
        int newPosition = piece->m_Position + direction[i];
        
        if (isOccupiedByWhite(chess, newPosition)             ||
           (i == 0 && isOccupiedByBlack(chess, newPosition))  ||
           ((i == 1 || i == 2) && chess->m_enPassantTarget != newPosition) ||
           ((i == 1 || i == 2) && isOccupiedByWhite(chess, chess->m_enPassantTarget - 8) && is_pawn(piece)))
            continue;
        
        vector_int_pushback(valid, newPosition);
    }

    if (rank != '2')
        return;

    int newPosition = piece->m_Position + 2 * direction[0];
    int prePosition = piece->m_Position + 1 * direction[0];
    
    if (isOccupiedByWhite(chess, newPosition) || isOccupiedByBlack(chess, newPosition) ||
        isOccupiedByWhite(chess, prePosition) || isOccupiedByBlack(chess, prePosition))
        return;
    
    vector_int_pushback(valid, newPosition);
}

void generatePawnMoves_black (Chess* chess, Piece* piece, vector_int* valid) {
    int direction[] = {
        +8, // down
        +9, // down-right
        +7  // down-left
    };

    char file = piece->m_Position % 8;
    char rank = 8 - piece->m_Position / 8 - 1 + '1';

    for (int i = 0; i < 3; ++i) {
        if (rank == '1')
            EXCEPTION("black pawn cannot exist on the 1st rank");
        
        if ((i == 1 && file == 'h') ||
            (i == 2 && file == 'a'))
            continue;
        
        int newPosition = piece->m_Position + direction[i];
        
        if (isOccupiedByBlack(chess, newPosition)             ||
           (i == 0 && isOccupiedByWhite(chess, newPosition))  ||
           ((i == 1 || i == 2) && chess->m_enPassantTarget != newPosition) ||
           ((i == 1 || i == 2) && isOccupiedByBlack(chess, chess->m_enPassantTarget + 8) && is_pawn(piece)))
            continue;
        
        vector_int_pushback(valid, newPosition);
    }

    if (rank != '7')
        return;

    int newPosition = piece->m_Position + 2 * direction[0];
    int prePosition = piece->m_Position + 1 * direction[0];
    
    if (isOccupiedByWhite(chess, newPosition) || isOccupiedByBlack(chess, newPosition) ||
        isOccupiedByWhite(chess, prePosition) || isOccupiedByBlack(chess, prePosition))
        return;
    
    if (isOccupiedByWhite(chess, newPosition) || isOccupiedByBlack(chess, newPosition))
        return;
    
    vector_int_pushback(valid, newPosition);
}

bool isOccupiedByWhite (Chess* chess, int position) {
    for (int i = 0; i < 16; ++i)
        if (chess->m_White[i].m_Position == position)
            return true;
    return false;
}

bool isOccupiedByBlack (Chess* chess, int position) {
    for (int i = 0; i < 16; ++i)
        if (chess->m_Black[i].m_Position == position)
            return true;
    return false;
}

#endif // chess_move_generation_h
