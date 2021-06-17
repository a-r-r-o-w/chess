#ifndef chess_types_h
#define chess_types_h

typedef int position;

enum piece_type {
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

enum file {
    FILE_A = 0,
    FILE_B = 1,
    FILE_C = 2,
    FILE_D = 3,
    FILE_E = 4,
    FILE_F = 5,
    FILE_G = 6,
    FILE_H = 7
};

enum rank {
    RANK_8 = 0,
    RANK_7 = 1,
    RANK_6 = 2,
    RANK_5 = 3,
    RANK_4 = 4,
    RANK_3 = 5,
    RANK_2 = 6,
    RANK_1 = 7
};

struct chess;
struct board;
struct piece;

char __symbol[] = {
    'K', // white_king
    'Q', // white_queen
    'R', // white_rook
    'B', // white_bishop
    'N', // white_knight
    'P', // white_pawn
    'k', // black_king
    'q', // black_queen
    'r', // black_rook
    'b', // black_bishop
    'n', // black_knight
    'p'  // black_pawn
};


char to_symbol (enum piece_type type) {
    return __symbol[type];
}

#endif // chess_types_h
