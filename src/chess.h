#ifndef chess_h
#define chess_h

#include "stdbool.h"

#include "exception.h"
#include "types.h"
#include "board.h"

void chess_constructor (struct chess*);
void set_board_position (struct chess*, const char*);

// FEN parsing helper functions

void parse_FEN (struct chess*, const char*);

void __parse_FEN_piece_placement (struct board*, const char*);
void __parse_FEN_active_color    (struct chess*, const char*);
void __parse_FEN_castling        (struct chess*, const char*);
void __parse_FEN_en_passant      (struct chess*, const char*);
void __parse_FEN_halfmoves       (struct chess*, const char*);
void __parse_FEN_fullmoves       (struct chess*, const char*);

int __split (const char*, char*);

struct chess {
    struct board board;

    bool is_white_turn;

    bool white_kingside_castle;
    bool black_kingside_castle;
    bool white_queenside_castle;
    bool black_queenside_castle;

    position en_passant_target;

    int halfmoves;
    int fullmoves;
};

void chess_constructor (struct chess* chess) {
    board_constructor(&chess->board);
}

void set_board_position (struct chess* chess, const char* FEN) {
    parse_FEN(chess, FEN);
}

// This does not validate the FEN string completely
// Only a few easy checks are implemented. Validating a FEN string completely can be quite
// a difficult task
// It is assumed that the string passed is correct
void parse_FEN (struct chess* chess, const char* FEN) {

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

       (https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation)
    */

    char piece_placement [100];
    char active_color    [10];
    char castling        [10];
    char en_passant      [10];
    char halfmoves       [10];
    char fullmoves       [10];

    // divide the string into parts and assign them according to FEN format
    int index = 0;
    index += 1 + __split(&FEN[index], piece_placement);
    index += 1 + __split(&FEN[index], active_color);
    index += 1 + __split(&FEN[index], castling);
    index += 1 + __split(&FEN[index], en_passant);
    index += 1 + __split(&FEN[index], halfmoves);
    index += 1 + __split(&FEN[index], fullmoves);

    // set the board configurations
    __parse_FEN_piece_placement (&chess->board, piece_placement);
    __parse_FEN_active_color    (chess, active_color);
    __parse_FEN_castling        (chess, castling);
    __parse_FEN_en_passant      (chess, en_passant);
    __parse_FEN_halfmoves       (chess, halfmoves);
    __parse_FEN_fullmoves       (chess, fullmoves);
}

// Splits the FEN string based on spaces
int __split (const char* FEN, char* fen_type) {
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

// Sets the pieces on the board
void __parse_FEN_piece_placement (struct board* board, const char* FEN) {
    // FEN indexing starts from a8 and ends at h1
    char rank = '8';
    char file = 'a';

    // starting indices for each piece
    int K = 0, Q = 1, R = 2, B = 4, N = 6, P = 8;
    int k = 0, q = 1, r = 2, b = 4, n = 6, p = 8;
    int count = 0;

    for (int i = 0; FEN[i] != '\0'; ++i) {
        char c = FEN[i];
        
        // '/' denotes the end of a rank (we've reached the h-file) and that we should proceed to the next rank
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
                case 'K': set_piece_position(&board->white[K++], file, rank); break;
                case 'Q': set_piece_position(&board->white[Q++], file, rank); break;
                case 'R': set_piece_position(&board->white[R++], file, rank); break;
                case 'B': set_piece_position(&board->white[B++], file, rank); break;
                case 'N': set_piece_position(&board->white[N++], file, rank); break;
                case 'P': set_piece_position(&board->white[P++], file, rank); break;

                default: exception("invalid FEN string", __FILE__, __LINE__);
            }
            ++file;
            ++count;
        }

        // small letters are used to denote the black pieces
        // valid small letters: k (king), q (queen), r (rook), b (bishop), n (knight), p (pawn)
        else if ('a' <= c && c <= 'z') {
            switch(c) {
                case 'k': set_piece_position(&board->black[k++], file, rank); break;
                case 'q': set_piece_position(&board->black[q++], file, rank); break;
                case 'r': set_piece_position(&board->black[r++], file, rank); break;
                case 'b': set_piece_position(&board->black[b++], file, rank); break;
                case 'n': set_piece_position(&board->black[n++], file, rank); break;
                case 'p': set_piece_position(&board->black[p++], file, rank); break;

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
void __parse_FEN_active_color (struct chess* chess, const char* FEN) {
    if (FEN[0] == 'w')
        chess->is_white_turn = true;
    else if (FEN[0] == 'b')
        chess->is_white_turn = false;
    else
        exception("invalid FEN string", __FILE__, __LINE__);
}

// sets castling priviliges for the two colors
void __parse_FEN_castling (struct chess* chess, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        switch (FEN[i]) {
            case 'K': chess->white_kingside_castle  = true; break;
            case 'k': chess->black_kingside_castle  = true; break;
            case 'Q': chess->white_queenside_castle = true; break;
            case 'q': chess->black_queenside_castle = true; break;
            case '-': break;

            default: exception("invalid FEN string", __FILE__, __LINE__);
        }
    }
}

// sets en_passant target after the previous player made a two-square pawn move
// target is the position "behind this pawn"
void __parse_FEN_en_passant (struct chess* chess, const char* FEN) {
    if (FEN[0] == '-' || FEN[1] == '-') {
        // no en passant target
        chess->en_passant_target = -1;
    }
    else {
        char file = FEN[0];
        char rank = FEN[1];
        
        position index = __indexify(file, rank);

        // number of possible positions on a chess board are 64
        // we index them from [0, 1, ..., 63]
        if (index < 0 || index > (64 - 1)) {
            exception("invalid position provided", __FILE__, __LINE__);
        }

        chess->en_passant_target = index;
    }
}

// sets number of moves played since the last capture or pawn advance
void __parse_FEN_halfmoves (struct chess* chess, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        chess->halfmoves *= 10;
        chess->halfmoves += FEN[i] - '0';
    }
}

// sets the number of complete sets of moves played by both players
// a single move consists of one piece move by white and one piece move by black
void __parse_FEN_fullmoves (struct chess* chess, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        chess->fullmoves *= 10;
        chess->fullmoves += FEN[i] - '0';
    }
}

#endif // chess_h
