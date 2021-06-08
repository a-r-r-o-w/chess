#ifndef chess_board_h
#define chess_board_h

#include "stdio.h"
#include "stdbool.h"

#include "exceptions.h"
#include "pieces.h"

typedef struct {
    // [16]: [K, Q, R, R, B, B, N, N, P, P, P, P, P, P, P, P]
    // K: King
    // Q: Queen
    // R: Rook
    // B: Bishop
    // N: Knight
    // P: Pawn
    piece white[16];
    piece black[16];

    bool is_white_turn;
} board;

// function prototypes ------------------------------------------------------------------------

void board_constructor (board*, const char*);

void parse_FEN                 (board*, const char*);
void parse_FEN_piece_placement (board*, const char*);

void display_board (board*, bool);

int  __count_trailing_zeroes (position);
void __place_piece           (char*, piece*);

// function definitions -----------------------------------------------------------------------

void board_constructor (board* board, const char* FEN) {
    // assign kings, queens, rooks, bishops, knights and pawns their symbols for each color

    piece_constructor(&board->white[0] , 'K');
    piece_constructor(&board->white[1] , 'Q');
    piece_constructor(&board->white[2] , 'R');
    piece_constructor(&board->white[3] , 'R');
    piece_constructor(&board->white[4] , 'B');
    piece_constructor(&board->white[5] , 'B');
    piece_constructor(&board->white[6] , 'N');
    piece_constructor(&board->white[7] , 'N');
    piece_constructor(&board->white[8] , 'P');
    piece_constructor(&board->white[9] , 'P');
    piece_constructor(&board->white[10], 'P');
    piece_constructor(&board->white[11], 'P');
    piece_constructor(&board->white[12], 'P');
    piece_constructor(&board->white[13], 'P');
    piece_constructor(&board->white[14], 'P');
    piece_constructor(&board->white[15], 'P');

    piece_constructor(&board->black[0] , 'k');
    piece_constructor(&board->black[1] , 'q');
    piece_constructor(&board->black[2] , 'r');
    piece_constructor(&board->black[3] , 'r');
    piece_constructor(&board->black[4] , 'b');
    piece_constructor(&board->black[5] , 'b');
    piece_constructor(&board->black[6] , 'n');
    piece_constructor(&board->black[7] , 'n');
    piece_constructor(&board->black[8] , 'p');
    piece_constructor(&board->black[9] , 'p');
    piece_constructor(&board->black[10], 'p');
    piece_constructor(&board->black[11], 'p');
    piece_constructor(&board->black[12], 'p');
    piece_constructor(&board->black[13], 'p');
    piece_constructor(&board->black[14], 'p');
    piece_constructor(&board->black[15], 'p');

    // Is it white's turn?
    // Assumed yes, but original value is set after parsing FEN string
    board->is_white_turn = true;

    // parse the FEN string and set the board position
    parse_FEN(board, FEN);
}

// This does not validate the FEN string completely
// Only a few easy checks are implemented. Validating a FEN string completely can be quite
// a difficult task
// It is assumed that the string passed is correct. If not, certain easily spottable mistakes
// can be detected here
void parse_FEN (board* board, const char* FEN) {

    /* A FEN string is a group of 6 space-separated sub-strings that each hold some information
       about a game of chess.

       The first part is the piece placement ordering.
       The second part is the color of the active player.
       The third part is the availability of castling (KQkq)
            - K denotes white can castle kingside and Q denotes white can castle queenside
            - k denotes black can castle kingside and q denotes black can castle queenside
       The fourth part denotes if an en-passant move is available. if not, it is set to '-'
       The fifth part denotes number of halfmove cycles since the last capture (50-move rule)
       The sixth part denotes number of full moves played. Incremented every time after black plays.

       (https://www.wikiwand.com/en/Forsyth%E2%80%93Edwards_Notation)
    */

    char piece_placement[100];
    // char active_color;
    // char castling[4];
    // char en_passant[2];
    // char halfmoves[4];
    // char fullmoves[4];

    int index = 0, offset = 0;
    while (FEN[index] != '\0') {
        if (FEN[index] == ' ')
            break;
        piece_placement[index - offset] = FEN[index];
        ++index;
    }
    piece_placement[index - offset] = '\0';

    parse_FEN_piece_placement(board, piece_placement);
}

void parse_FEN_piece_placement (board* board, const char* FEN) {
    // FEN indexing starts from a8 and ends at h1
    char rank = '8';
    char file = 'a';

    int K = 0, Q = 1, R = 2, B = 4, N = 6, P = 8;
    int k = 0, q = 1, r = 2, b = 4, n = 6, p = 8;

    for (int i = 0; FEN[i] != '\0'; ++i) {
        char c = FEN[i];
        
        // '/' denotes the end of a file and that we should proceed to next rank
        if (c == '/') {
            --rank;
            file = 'a';
        }
        
        // the only valid files are 'a'-'h'
        else if (file > 'h')
            exception("invalid FEN string", __FILE__, __LINE__);
        
        // capital letters are used to denote the white pieces
        // valid capital letters: K (king), Q (queen), R (rook), B (bishop), N (knight), P (pawn)
        else if ('A' <= c && c <= 'Z') {
            switch(c) {
                case 'K': piece_set(&board->white[K++], file, rank); break;
                case 'Q': piece_set(&board->white[Q++], file, rank); break;
                case 'R': piece_set(&board->white[R++], file, rank); break;
                case 'B': piece_set(&board->white[B++], file, rank); break;
                case 'N': piece_set(&board->white[N++], file, rank); break;
                case 'P': piece_set(&board->white[P++], file, rank); break;

                default: exception("invalid FEN string", __FILE__, __LINE__);
            }
            ++file;
        }

        // small letters are used to denote the black pieces
        // valid small letters: k (king), q (queen), r (rook), b (bishop), n (knight), p (pawn)
        else if ('a' <= c && c <= 'z') {
            switch(c) {
                case 'k': piece_set(&board->black[k++], file, rank); break;
                case 'q': piece_set(&board->black[q++], file, rank); break;
                case 'r': piece_set(&board->black[r++], file, rank); break;
                case 'b': piece_set(&board->black[b++], file, rank); break;
                case 'n': piece_set(&board->black[n++], file, rank); break;
                case 'p': piece_set(&board->black[p++], file, rank); break;

                default: exception("invalid FEN string", __FILE__, __LINE__);
            }
            ++file;
        }

        // numbers are used to denote the number of continuous empty squares in a rank without pieces
        else if ('1' <= c && c <= '8') {
            file += c - '0';

            // cannot have more empty spaces than size of the board
            // 'h' + 1 is valid because the next character in the FEN string might be '/'
            if (file > 'h' + 1)
                exception("invalid FEN string", __FILE__, __LINE__);
        }
        else
            exception("invalid FEN string", __FILE__, __LINE__);
    }

    // after iterating over the 8 ranks and 8 files, we must be at rank 0 and file 'i' (i.e. outside the board)
    // if not the FEN string was invalid (sum of numbers + piece_count != 64)
    printf("%c%c\n", file, rank);
    if (file != 'i' && rank != '0')
        exception("invalid FEN string", __FILE__, __LINE__);
}

void display_board (board* board, bool is_white_perspective) {
    // create and assign the chessboard with empty position values
    char pretty_board[64];
    for (int i = 0; i < 64; ++i)
        pretty_board[i] = '-';
    
    // place the pieces
    for (int i = 0; i < 16; ++i) {
        __place_piece(pretty_board, &board->white[i]);
        __place_piece(pretty_board, &board->black[i]);
    }

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int index;
            
            if (is_white_perspective)
                index = 8 * i + j;
            else
                index = 8 * (8 - i - 1) + (8 - j - 1);
            printf("%c ", pretty_board[index]);
        }
        printf("\n");
    }
}

int __count_trailing_zeroes (position position) {
    int ctz = 0;
    for (int bit = 0; bit < 64; ++bit, ++ctz)
        if (position & (1ull << bit))
            break;
    return ctz;
}

void __place_piece (char* pretty_board, piece* piece) {
    int ctz = __count_trailing_zeroes(piece->position);
    pretty_board[ctz] = piece->symbol;
}

#endif
