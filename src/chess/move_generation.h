#ifndef chess_move_generation_h
#define chess_move_gereration_h

#include "stdbool.h"

#include "exception.h"
#include "chess.h"
#include "piece.h"
#include "vector.h"

/* Function declarations ------------------------------------------------------------------ */

void generateMoves               (Chess*, Piece*, vector_int*);
void generateSlidingMoves        (Chess*, Piece*, vector_int*);
void generateKnightMoves         (Chess*, Piece*, vector_int*);
void generatePawnMoves_white     (Chess*, Piece*, vector_int*);
void generatePawnMoves_black     (Chess*, Piece*, vector_int*);
void genAttackingMoves           (Chess*, Piece*, vector_int*);
void genAttackingSlidingMoves    (Chess*, Piece*, vector_int*);
void genAttackingKnightMoves     (Chess*, Piece*, vector_int*);
void genAttackingPawnMoves_white (Chess*, Piece*, vector_int*);
void genAttackingPawnMoves_black (Chess*, Piece*, vector_int*);

void filterInvalidMoves (Chess*, Piece*, vector_int*, vector_int*);

bool isWhiteKingInCheck (Chess*);
bool isBlackKingInCheck (Chess*);

bool isOccupiedByWhite (Chess*, int);
bool isOccupiedByBlack (Chess*, int);

/* Object types --------------------------------------------------------------------------- */

/* Function definitions ------------------------------------------------------------------- */

void generateMoves (Chess* chess, Piece* piece, vector_int* valid) {
    if (piece->m_Position == -1)
        return;
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

void generateSlidingMoves (Chess* chess, Piece* piece, vector_int* moves) {
    vector_int testMoves;
    vector_int_constructor(&testMoves, 0, 0);

    genAttackingSlidingMoves(chess, piece, &testMoves);
    filterInvalidMoves(chess, piece, &testMoves, moves);

    vector_int_destructor(&testMoves);
}

void generateKnightMoves (Chess* chess, Piece* piece, vector_int* moves) {
    vector_int testMoves;
    vector_int_constructor(&testMoves, 0, 0);

    genAttackingKnightMoves(chess, piece, &testMoves);
    filterInvalidMoves(chess, piece, &testMoves, moves);

    vector_int_destructor(&testMoves);
}

void generatePawnMoves_white (Chess* chess, Piece* piece, vector_int* moves) {
    vector_int testMoves;
    vector_int_constructor(&testMoves, 0, 0);

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
        
        if ((i == 0 && !isOccupiedByWhite(chess, newPosition) && !isOccupiedByBlack(chess, newPosition)) ||
            ((i == 1 || i == 2) && isOccupiedByBlack(chess, newPosition)))
            vector_int_pushback(&testMoves, newPosition);
    }

    if (rank == '2') {
        int newPosition = piece->m_Position + 2 * direction[0];
        int prePosition = piece->m_Position + 1 * direction[0];
        
        if (isOccupiedByWhite(chess, newPosition) || isOccupiedByBlack(chess, newPosition) ||
            isOccupiedByWhite(chess, prePosition) || isOccupiedByBlack(chess, prePosition))
            return;
        
        vector_int_pushback(&testMoves, newPosition);
    }

    filterInvalidMoves(chess, piece, &testMoves, moves);

    vector_int_destructor(&testMoves);
}

void generatePawnMoves_black (Chess* chess, Piece* piece, vector_int* moves) {
    vector_int testMoves;
    vector_int_constructor(&testMoves, 0, 0);

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

        if ((i == 0 && !isOccupiedByWhite(chess, newPosition) && !isOccupiedByBlack(chess, newPosition)) ||
            ((i == 1 || i == 2) && isOccupiedByWhite(chess, newPosition)))
            vector_int_pushback(&testMoves, newPosition);
    }

    if (rank == '7') {
        int newPosition = piece->m_Position + 2 * direction[0];
        int prePosition = piece->m_Position + 1 * direction[0];
        
        if (isOccupiedByWhite(chess, newPosition) || isOccupiedByBlack(chess, newPosition) ||
            isOccupiedByWhite(chess, prePosition) || isOccupiedByBlack(chess, prePosition))
            return;
        
        vector_int_pushback(&testMoves, newPosition);
    }

    filterInvalidMoves(chess, piece, &testMoves, moves);

    vector_int_destructor(&testMoves);
}

void filterInvalidMoves (Chess* chess, Piece* piece, vector_int* testMoves, vector_int* goodMoves) {
    bool is_white = isWhitePiece(piece);

    int originalPosition = piece->m_Position;

    for (int i = 0; i < testMoves->size; ++i) {
        piece->m_Position = testMoves->data[i];

        if (is_white) {
            if (!isWhiteKingInCheck(chess))
                vector_int_pushback(goodMoves, testMoves->data[i]);
        }
        else {
            if (!isBlackKingInCheck(chess))
                vector_int_pushback(goodMoves, testMoves->data[i]);
        }

        piece->m_Position = originalPosition;
    }
}

bool isWhiteKingInCheck (Chess* chess) {
    bool isAttacked[64];
    for (int i = 0; i < 64; ++i)
        isAttacked[i] = false;
    
    for (int i = 0; i < 16; ++i) {
        vector_int moves;
        vector_int_constructor(&moves, 0, 0);
        
        genAttackingMoves(chess, &chess->m_Black[i], &moves);
        for (int j = 0; j < moves.size; ++j)
            isAttacked[moves.data[j]] = true;
        
        vector_int_destructor(&moves);
    }

    for (int i = 0; i < 16; ++i)
        if (is_king(&chess->m_White[i]))
            return isAttacked[chess->m_White[i].m_Position];
    
    return false;
}

bool isBlackKingInCheck (Chess* chess) {
    bool isAttacked[64];
    for (int i = 0; i < 64; ++i)
        isAttacked[i] = false;
    
    for (int i = 0; i < 16; ++i) {
        vector_int moves;
        vector_int_constructor(&moves, 0, 0);
        
        genAttackingMoves(chess, &chess->m_White[i], &moves);
        for (int j = 0; j < moves.size; ++j) {
            isAttacked[moves.data[j]] = true;
        }
        
        vector_int_destructor(&moves);
    }

    for (int i = 0; i < 16; ++i)
        if (is_king(&chess->m_Black[i]))
            return isAttacked[chess->m_Black[i].m_Position];
    
    return false;
}

void genAttackingMoves (Chess* chess, Piece* piece, vector_int* moves) {
    if (piece->m_Position == -1)
        return;
    
    if (is_pawn(piece)) {
        if (isWhitePiece(piece))
            genAttackingPawnMoves_white(chess, piece, moves);
        else
            genAttackingPawnMoves_black(chess, piece, moves);
    }
    else if (is_knight(piece))
        genAttackingKnightMoves(chess, piece, moves);
    else
        genAttackingSlidingMoves(chess, piece, moves);
}

void genAttackingSlidingMoves (Chess* chess, Piece* piece, vector_int* moves) {
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
    
    char file = piece->m_Position % 8 + 'a';
    char rank = 8 - piece->m_Position / 8 - 1 + '1';
    
    for (int i = begin; i < end; ++i) {
        for (int times = 0; times < count; ++times) {

            if ((i == 0 && file == 'a') ||
                (i == 1 && file == 'h') ||
                (i == 2 && rank == '8') ||
                (i == 3 && rank == '1') ||
                (i == 4 && (file == 'a' || rank == '8')) ||
                (i == 5 && (file == 'h' || rank == '8')) ||
                (i == 6 && (file == 'a' || rank == '1')) ||
                (i == 7 && (file == 'h' || rank == '1')))
                break;
            
            int newPosition = piece->m_Position + (times + 1) * directions[i];
            char newfile = newPosition % 8 + 'a';
            char newrank = 8 - newPosition / 8 - 1 + '1';

            if (newPosition < 0 || newPosition >= 64)
                continue;

            if ((isWhitePiece(piece) && isOccupiedByWhite(chess, newPosition)) ||
                (isBlackPiece(piece) && isOccupiedByBlack(chess, newPosition)))
                break;

            vector_int_pushback(moves, newPosition);

            if ((i == 0 && newfile == 'a') ||
                (i == 1 && newfile == 'h') ||
                (i == 2 && newrank == '8') ||
                (i == 3 && newrank == '1') ||
                (i == 4 && (newfile == 'a' || newrank == '8')) ||
                (i == 5 && (newfile == 'h' || newrank == '8')) ||
                (i == 6 && (newfile == 'a' || newrank == '1')) ||
                (i == 7 && (newfile == 'h' || newrank == '1')))
                break;

            if ((isWhitePiece(piece) && isOccupiedByBlack(chess, newPosition)) ||
                (isBlackPiece(piece) && isOccupiedByWhite(chess, newPosition)))
                break;
        }
    }
}

void genAttackingKnightMoves (Chess* chess, Piece* piece, vector_int* moves) {
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
            
            vector_int_pushback(moves, newPosition);
        }
    }
}

void genAttackingPawnMoves_white (Chess* chess, Piece* piece, vector_int* moves) {
    int direction[] = {
        -9, // up-left
        -7  // up-right
    };

    char file = piece->m_Position % 8 + 'a';
    char rank = 8 - piece->m_Position / 8 - 1 + '1';

    for (int i = 0; i < 2; ++i) {
        if (rank == '8')
            EXCEPTION("white pawn cannot exist on the 8th rank");
        
        if ((i == 0 && file == 'a') ||
            (i == 1 && file == 'h'))
            continue;
        
        int newPosition = piece->m_Position + direction[i];
        
        vector_int_pushback(moves, newPosition);
    }
}

void genAttackingPawnMoves_black (Chess* chess, Piece* piece, vector_int* moves) {
    int direction[] = {
        +9, // down-right
        +7  // down-left
    };

    char file = piece->m_Position % 8 + 'a';
    char rank = 8 - piece->m_Position / 8 - 1 + '1';

    for (int i = 0; i < 2; ++i) {
        if (rank == '1')
            EXCEPTION("black pawn cannot exist on the 8th rank");
        
        if ((i == 0 && file == 'h') ||
            (i == 1 && file == 'a'))
            continue;
        
        int newPosition = piece->m_Position + direction[i];
        
        vector_int_pushback(moves, newPosition);
    }
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
