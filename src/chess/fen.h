#ifndef chess_fen_h
#define chess_fen_h

#include "exception.h"
#include "chess.h"
#include "constants.h"

/* Function declarations ------------------------------------------------------------------ */

void ParseFEN                (Chess*, const char*);
void ParseFEN_PiecePlacement (Chess*, const char*);
void ParseFEN_ActiveColor    (Chess*, const char*);
void ParseFEN_Castling       (Chess*, const char*);
void ParseFEN_enPassant      (Chess*, const char*);
void ParseFEN_halfMoves      (Chess*, const char*);
void ParseFEN_fullMoves      (Chess*, const char*);

static int Split (const char*, char*);

/* Object types --------------------------------------------------------------------------- */

/* Function definitions ------------------------------------------------------------------- */

/**
 * Parse a FEN string
 *   Note that this does not validate a FEN string completely
 *   A few simple checks are present
 *   It is assumed that the FEN string passes is correct
 */
void ParseFEN (Chess* chess, const char* FEN) {
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
    index += 1 + Split(&FEN[index], piece_placement);
    index += 1 + Split(&FEN[index], active_color);
    index += 1 + Split(&FEN[index], castling);
    index += 1 + Split(&FEN[index], en_passant);
    index += 1 + Split(&FEN[index], halfmoves);
    index += 1 + Split(&FEN[index], fullmoves);

    // set the board configurations
    ParseFEN_PiecePlacement (chess, piece_placement);
    ParseFEN_ActiveColor    (chess, active_color);
    ParseFEN_Castling       (chess, castling);
    ParseFEN_enPassant      (chess, en_passant);
    ParseFEN_halfMoves      (chess, halfmoves);
    ParseFEN_fullMoves      (chess, fullmoves);
}

/**
 * Sets up the piece information by parsing a FEN string
 */
void ParseFEN_PiecePlacement (Chess* chess, const char* FEN) {
    // FEN indexing starts from a8 and ends at h1
    char rank = '8';
    char file = 'a';

    // starting indices for each piece
    int W = 0;
    int B = 0;
    int count = 0;

    for (int i = 0; FEN[i] != '\0'; ++i) {
        char c = FEN[i];
        
        // '/' denotes the end of a rank (we've reached the h-file) and that we should proceed to the next rank
        if (c == '/') {
            rank -= 1;
            file = 'a';
        }
        
        // the only valid files are 'a'-'h'
        else if (file > 'h') {
            EXCEPTION("invalid FEN string");
        }
        
        // capital letters are used to denote the white pieces
        // valid capital letters: K (king), Q (queen), R (rook), B (bishop), N (knight), P (pawn)
        else if ('A' <= c && c <= 'Z') {
            switch(c) {
                case 'K': Piece_Set(&chess->m_White[W++], file, rank, white_king);   break;
                case 'Q': Piece_Set(&chess->m_White[W++], file, rank, white_queen);  break;
                case 'R': Piece_Set(&chess->m_White[W++], file, rank, white_rook);   break;
                case 'B': Piece_Set(&chess->m_White[W++], file, rank, white_bishop); break;
                case 'N': Piece_Set(&chess->m_White[W++], file, rank, white_knight); break;
                case 'P': Piece_Set(&chess->m_White[W++], file, rank, white_pawn);   break;

                default: { EXCEPTION("invalid FEN string"); }
            }
            ++file;
            ++count;
        }

        // small letters are used to denote the black pieces
        // valid small letters: k (king), q (queen), r (rook), b (bishop), n (knight), p (pawn)
        else if ('a' <= c && c <= 'z') {
            switch(c) {
                case 'k': Piece_Set(&chess->m_Black[B++], file, rank, black_king);   break;
                case 'q': Piece_Set(&chess->m_Black[B++], file, rank, black_queen);  break;
                case 'r': Piece_Set(&chess->m_Black[B++], file, rank, black_rook);   break;
                case 'b': Piece_Set(&chess->m_Black[B++], file, rank, black_bishop); break;
                case 'n': Piece_Set(&chess->m_Black[B++], file, rank, black_knight); break;
                case 'p': Piece_Set(&chess->m_Black[B++], file, rank, black_pawn);   break;

                default: { EXCEPTION("invalid FEN string"); }
            }
            ++file;
            ++count;
        }

        // numbers are used to denote the number of continuous empty squares in a rank without pieces
        else if ('1' <= c && c <= '8') {
            file  += c - '0';
            count += c - '0';

            // cannot have more empty spaces than size of the board
            // 'h' + 1 is valid because the next character in the FEN string might be '/'
            if (file > 'h' + 1) {
                EXCEPTION("invalid FEN string");
            }
        }
        else {
            EXCEPTION("invalid FEN string");
        }
    }

    // sum of numbers/empty_spaces + piece_count should be 64
    if (count != 64) {
        EXCEPTION("invalid FEN string");
    }
}

// sets active color
void ParseFEN_ActiveColor (Chess* chess, const char* FEN) {
    if (FEN[0] == 'w')
        chess->m_IsWhiteTurn = true;
    else if (FEN[0] == 'b')
        chess->m_IsWhiteTurn = false;
    else {
        EXCEPTION("invalid FEN string");
    }
}

// sets castling priviliges for the two colors
void ParseFEN_Castling (Chess* chess, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        switch (FEN[i]) {
            case 'K': chess->m_CanWhiteCastleK = true; break;
            case 'k': chess->m_CanBlackCastleK = true; break;
            case 'Q': chess->m_CanWhiteCastleQ = true; break;
            case 'q': chess->m_CanBlackCastleQ = true; break;
            case '-': break;

            default: { EXCEPTION("invalid FEN string") };
        }
    }
}

// sets en passant target after the previous player made a two-square pawn move
// target is the position "behind this pawn"
void ParseFEN_enPassant (Chess* chess, const char* FEN) {
    if (FEN[0] == '-' || FEN[1] == '-') {
        // no en passant target
        chess->m_enPassantTarget = -1;
    }
    else {
        char file = FEN[0];
        char rank = FEN[1];
        
        int index = indexify(file, rank);

        // number of possible positions on a chess board are 64
        // we index them from [0, 1, ..., 63]
        if (index < 0 || index > (64 - 1)) {
            EXCEPTION("invalid position provided");
        }

        chess->m_enPassantTarget = index;
    }
}

// sets number of moves played since the last capture or pawn advance
void ParseFEN_halfMoves (Chess* chess, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        chess->m_halfMoves *= 10;
        chess->m_halfMoves += FEN[i] - '0';
    }
}

// sets the number of complete sets of moves played by both players
// a single move consists of one piece move by white and one piece move by black
void ParseFEN_fullMoves (Chess* chess, const char* FEN) {
    for (int i = 0; FEN[i] != '\0'; ++i) {
        chess->m_fullMoves *= 10;
        chess->m_fullMoves += FEN[i] - '0';
    }
}

/**
 * Splits a string into substrings based on spaces
 */
static int Split (const char* FEN, char* fen_type) {
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

#endif // chess_fen_h
