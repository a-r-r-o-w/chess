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

    // whose turn is it?
    bool is_white_turn;

    // castling privileges for each color on each side
    bool white_kingside_castle;
    bool black_kingside_castle;
    bool white_queenside_castle;
    bool black_queenside_castle;

    // is there an en_passant available in the position?
    bool can_en_passant;

    // en passant target square
    // if a pawn made a two-square move, the position "behind" the pawn is the target square
    // the pawn can only be captured if can_en_passant is set to "true" and the player chooses to do so
    char en_passant_target[2];

    // number of moves played since the last capture or pawn advance
    int halfmoves;

    // number of full moves played. it is incremented after black's turn and starts initially at 1
    int fullmoves;
} board;

// function prototypes ------------------------------------------------------------------------

void board_constructor (board*, const char*);

void display_board (board*, bool);

int  __count_trailing_zeroes (position);
void __place_piece           (char*, piece*);

void parse_FEN                 (board*     , const char*);
int  __parse_FEN_split         (const char*, char*      );
void parse_FEN_piece_placement (board*     , const char*);
void parse_FEN_active_color    (board*     , const char*);
void parse_FEN_castling        (board*     , const char*);
void parse_FEN_en_passant      (board*     , const char*);
void parse_FEN_halfmoves       (board*     , const char*);
void parse_FEN_fullmoves       (board*     , const char*);

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

    // is it white's turn?
    // assumed yes, but original value is set after parsing FEN string
    board->is_white_turn = true;

    // should be set through FEN string
    board->white_kingside_castle  = false;
    board->black_kingside_castle  = false;
    board->white_queenside_castle = false;
    board->black_queenside_castle = false;

    // should be set through FEN string
    board->can_en_passant = false;
    board->en_passant_target[0] = '-';
    board->en_passant_target[1] = '-';

    board->halfmoves = 0;
    board->fullmoves = 1;

    // parse the FEN string and set the board position
    parse_FEN(board, FEN);
}

// display the pretty board!
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

// helper function to count number of trailing zero bits in the binary
// representation of a number
int __count_trailing_zeroes (position position) {
    int ctz = 0;
    for (int bit = 0; bit < 64; ++bit, ++ctz)
        if (position & (1ull << bit))
            break;
    return ctz;
}

// helper function to assign the board a symbol based on where a piece is placed
void __place_piece (char* pretty_board, piece* piece) {
    int ctz = __count_trailing_zeroes(piece->position);
    pretty_board[ctz] = piece->symbol;
}

// FEN parser

// this does not validate the FEN string completely
// only a few easy checks are implemented. validating a FEN string completely can be quite
// a difficult task
// it is assumed that the string passed is correct. If not, certain easily spottable mistakes
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

    char piece_placement [100];
    char active_color    [10];
    char castling        [10];
    char en_passant      [10];
    char halfmoves       [10];
    char fullmoves       [10];

    // divide the string into parts and assign them according to FEN format
    int index = 0;
    index += 1 + __parse_FEN_split(&FEN[index], piece_placement);
    index += 1 + __parse_FEN_split(&FEN[index], active_color);
    index += 1 + __parse_FEN_split(&FEN[index], castling);
    index += 1 + __parse_FEN_split(&FEN[index], en_passant);
    index += 1 + __parse_FEN_split(&FEN[index], halfmoves);
    index += 1 + __parse_FEN_split(&FEN[index], fullmoves);

    // set the board configurations
    parse_FEN_piece_placement (board, piece_placement);
    parse_FEN_active_color    (board, active_color);
    parse_FEN_castling        (board, castling);
    parse_FEN_en_passant      (board, en_passant);
    parse_FEN_halfmoves       (board, halfmoves);
    parse_FEN_fullmoves       (board, fullmoves);
}

// splits the FEN string based on spaces
// note that the FEN string must be valid with only single spaces between different parts
int __parse_FEN_split (const char* FEN, char* fen_type) {
    int index = 0;
    while (FEN[index] != '\0') {
        if (FEN[index] == ' ')
            break;
        fen_type[index] = FEN[index];
        ++index;
    }
    fen_type[index] = '\0';
    return index;
}

void parse_FEN_piece_placement (board* board, const char* FEN) {
    // FEN indexing starts from a8 and ends at h1
    char rank = '8';
    char file = 'a';

    int K = 0, Q = 1, R = 2, B = 4, N = 6, P = 8;
    int k = 0, q = 1, r = 2, b = 4, n = 6, p = 8;
    int count = 0;

    for (int i = 0; FEN[i] != '\0'; ++i) {
        char c = FEN[i];
        
        // '/' denotes the end of a file and that we should proceed to next rank
        if (c == '/') {
            rank -= 1;
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
            ++count;
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
            ++count;
        }

        // numbers are used to denote the number of continuous empty squares in a rank without pieces
        else if ('1' <= c && c <= '8') {
            file += c - '0';
            count += c - '0';

            // cannot have more empty spaces than size of the board
            // 'h' + 1 is valid because the next character in the FEN string might be '/'
            if (file > 'h' + 1)
                exception("invalid FEN string", __FILE__, __LINE__);
        }
        else
            exception("invalid FEN string", __FILE__, __LINE__);
    }

    // sum of numbers/empty_spaces + piece_count should be 64
    if (count != 64)
        exception("invalid FEN string", __FILE__, __LINE__);
}

// sets active color
void parse_FEN_active_color (board* board, const char* FEN) {
    if (FEN[0] == 'w')
        board->is_white_turn = true;
    else if (FEN[0] == 'b')
        board->is_white_turn = false;
    else
        exception("invalid FEN string", __FILE__, __LINE__);
}

// sets castling priviliges for the two colors
void parse_FEN_castling (board* board, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        switch (FEN[i]) {
            case 'K': board->white_kingside_castle  = true; break;
            case 'k': board->black_kingside_castle  = true; break;
            case 'Q': board->white_queenside_castle = true; break;
            case 'q': board->black_queenside_castle = true; break;
            case '-': break;

            default: exception("invalid FEN string", __FILE__, __LINE__);
        }
    }
}

// sets en_passant target after the previous player made a two-square pawn move
// target is the position "behind this pawn"
void parse_FEN_en_passant (board* board, const char* FEN) {
    
    // note that can_en_passant cannot be determined by looking at a position. we must know what
    // the last player's move was
    board->can_en_passant = false;

    if (FEN[0] == '-' || FEN[1] == '-') {
        board->en_passant_target[0] = '-';
        board->en_passant_target[1] = '-';
    }
    else {
        board->en_passant_target[0] = FEN[0];

        // "behind" the pawn
        if (board->is_white_turn)
            board->en_passant_target[1] = FEN[1] + 1;
        else
            board->en_passant_target[1] = FEN[1] - 1;
    }
}

// sets number of moves played since the last capture or pawn advance
void parse_FEN_halfmoves (board* board, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        board->halfmoves *= 10;
        board->halfmoves += FEN[i] - '0';
    }
}

// sets the number of complete sets of moves played by both players
// a single move consists of one piece move by white and one piece move by black
void parse_FEN_fullmoves (board* board, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        board->fullmoves *= 10;
        board->fullmoves += FEN[i] - '0';
    }
}

#endif
